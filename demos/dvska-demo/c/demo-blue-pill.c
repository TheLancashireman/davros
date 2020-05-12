/* demo-blue-pill.c - hardware-specific functions for davroska demo
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-stdio.h>
#include <dv-string.h>

#include <dv-stm32-rcc.h>
#include <dv-nvic.h>
#include <dv-stm32-uart.h>

#define DV_SPSEL		0x02
#define DV_PENDSVSET	0x10000000
#define DV_PENDSVCLR	0x08000000
/*                        12345678 */

extern unsigned dv_start_data, dv_end_data, dv_start_bss, dv_end_bss, dv_idata;
extern unsigned dv_pstacktop;
extern unsigned dv_stacktop;

extern int main(int argc, char **argv);
extern void switchToPsp(unsigned *psp, unsigned *msp, unsigned control, void (*fp)(void));

extern void preemptFrame(void);
extern void preemptFrame_end(void);

void dumpPstack(void);

/* dv_init_data() - initialise variables
 *
 * Initialises all variables from the flash image (.data) or to zero (.bss)
*/
void dv_init_data(void)
{
	unsigned *s = &dv_idata;
	unsigned *d = &dv_start_data;

	while ( d < &dv_end_data )
	{
		*d++ = *s++;
	}

	d = &dv_start_bss;
	while ( d < &dv_end_bss )
	{
		*d++ = 0x00;
	}
}

/* Mapping functions for console
*/
int uart1_putc(int c)
{
	dv_stm32_uart_putc(&dv_uart1, c);
	return 1;
}

int uart1_getc(void)
{
	return dv_stm32_uart_getc(&dv_uart1);
}

int uart1_isrx(void)
{
	return dv_stm32_uart_isrx(&dv_uart1);
}

int uart1_istx(void)
{
	return dv_stm32_uart_istx(&dv_uart1);
}

/* sysinfo() - print some information about the state of the uC
*/
void sysinfo(void)
{
	dv_printf("Sysinfo:\n");
	dv_printf("SP        = 0x%02x\n", dv_get_sp());
	dv_printf("mainSP    = 0x%02x\n", dv_get_msp());
	dv_printf("threadSP  = 0x%02x\n", dv_get_psp());
	dv_printf("PRIMASK   = 0x%02x\n", dv_get_primask());
	dv_printf("FAULTMASK = 0x%02x\n", dv_get_faultmask());
	dv_printf("BASEPRI   = 0x%02x\n", dv_get_basepri());
	dv_printf("CONTROL   = 0x%02x\n", dv_get_control());
	dv_printf("XPSR      = 0x%02x\n", dv_get_xpsr());
}

/* dv_reset2() - call main() after switching to thread stack
*/
void dv_reset2(void)
{
	sysinfo();

	(void)main(0, DV_NULL);
}

/* dv_reset() - entry point from the reset vector
 *
 * SP has been initialised, but that's all
*/
void dv_reset(void)
{
	/* Initialise the PLL: 72 MHz
	*/
	dv_rcc_init();

	/* Initialise variables
	*/
	dv_init_data();

	/* Initialise the interrupt controllers
	*/
	dv_nvic_init();

	/* Initialise uart1 and connect it to the stdio functions
	*/
	(void)dv_stm32_uart_init(&dv_uart1, 115200, "8N1");
	dv_consoledriver.putc = uart1_putc;
	dv_consoledriver.getc = uart1_getc;
	dv_consoledriver.istx = uart1_istx;
	dv_consoledriver.isrx = uart1_isrx;

	sysinfo();

	/* It would be possible to pass main() as the function pointer here,
	 * but for the time being we'll use an intermediate function so that we can find out
	 * what's going on.
	*/
	switchToPsp(&dv_pstacktop, &dv_stacktop, (dv_get_control() | DV_SPSEL), &dv_reset2);
}

void dv_panic_return_from_switchcall_function(void)
{
	dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_scheduler, "Oops! The task wrapper returned");
}

/* Below this line are stub functions to satisfy the vector addresses
*/
void dv_nmi(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! An NMI occurred");
}

void dv_hardfault(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! A hardfault trap occurred");
}

void dv_memfault(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! A memfault trap occurred");
}

void dv_busfault(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! A busfault trap occurred");
}

void dv_usagefault(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! A usage fault occurred");
}

void dv_svctrap(void)
{
	dv_printf("dv_svc()\n");
	sysinfo();
	dumpPstack();
	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();

	sysinfo();

	if ( (psp[6] & ~0x01) == (dv_u32_t)&preemptFrame_end )
	{
		psp += 8;
		dv_set_psp((dv_u32_t)psp);
	}
	else
	{
		dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! Unexpected SVC");
	}
}

void xyz(void)
{
	dv_printf("xyz()\n");
	sysinfo();
}

void dv_pendsvtrap(void)
{
	dv_printf("dv_pendsvtrap()\n");
	sysinfo();
	dumpPstack();

	dv_m3scr.icsr = DV_PENDSVCLR;

	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();
	psp -= 8;
	dv_set_psp((dv_u32_t)psp);

	psp[0] = ((dv_u32_t)&xyz) | 0x01;				/* r0 */
	psp[1] = psp[2] = psp[3] = psp[4] = 0;			/* r1-r3, r12 */
	psp[5] = 0;										/* lr */
	psp[6] = ((dv_u32_t)&preemptFrame) | 0x01;		/* pc */
	psp[7] = psp[15];								/* xPSR */

	dv_printf("dv_pendsvtrap() after pushing preeption frame\n");
	sysinfo();
	dumpPstack();
}

void dv_systickirq(void)
{
	dv_printf("dv_systickirq()\n");
	for (;;)	{ }
}

void dv_irq(void)
{
	dv_printf("dv_irq()\n");
	sysinfo();
	dumpPstack();
	dv_m3scr.icsr = DV_PENDSVSET;
}

void dumpPstack(void)
{
	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();

	for ( int i=15; i>=0; i-- )
	{
		dv_printf("psp[%02d] (0x%08x) = 0x%08x\n", i, (dv_u32_t)&psp[i], psp[i]);
	}
}

void dv_unknowntrap(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! An undocumented trap occurred");
}

