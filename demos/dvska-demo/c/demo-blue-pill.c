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

#include <davroska-inline.h>

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

	/* Initialise the exception priorities
	*/
	dv_m3scr.shpr[0] = 0x0;		/* UsageFault, BusFault, MemManage and [reserved]  all at highest priority */
	dv_m3scr.shpr[1] = 0x0;		/* SVC and [reserved x 3] all at highest priority */
	dv_m3scr.shpr[2] = 0xffff0000;	/* SysTick/PendSV at lowest priority, Debug and [reserved] at highest */

	/* Initialise the interrupt controller
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
#if 0
	sysinfo();
	dumpPstack();
#endif
	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();

	if ( (psp[6] & ~0x01) == (dv_u32_t)&preemptFrame_end )
	{
		psp += 8;
		dv_set_psp((dv_u32_t)psp);
		dv_setirqlevel(0);
	}
	else
	{
		dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! Unexpected SVC");
	}
}

void xyz(void)
{
	dv_printf("xyz() - begin\n");
	dv_printf("xyz() - current exe = %s\n", dv_exe[dv_currentexe].name);
#if 0
	sysinfo();
#endif

	/* Now run all queued executables down to saved priority
	*/
	dv_runqueued_onkernelstack(dv_maxtaskprio, dv_exe[dv_currentexe].currprio, DV_INTENABLED);

	dv_printf("xyz() - run queue finished\n");
#if 1
	sysinfo();
#endif

	/* When all higher-priority activity is done, back to the original caller
	*/
	dv_setirqlevel(15);
	(void)dv_restore(DV_INTENABLED);
}

void dv_pendsvtrap(void)
{
	dv_printf("dv_pendsvtrap()\n");
#if 0
	sysinfo();
	dumpPstack();
#endif

	dv_m3scr.icsr = DV_PENDSVCLR;

	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();
	psp -= 8;
	dv_set_psp((dv_u32_t)psp);

	psp[0] = ((dv_u32_t)&xyz) | 0x01;				/* r0 */
	psp[1] = psp[2] = psp[3] = psp[4] = 0;			/* r1-r3, r12 */
	psp[5] = 0;										/* lr */
	psp[6] = ((dv_u32_t)&preemptFrame) | 0x01;		/* pc */
	psp[7] = psp[15];								/* xPSR */

	dv_printf("dv_highestprio = %d; level = %d, dv_currentlocklevel = %d\n",
					dv_highestprio, dv_queue[dv_highestprio].level, dv_currentlocklevel);
	dv_setirqlevel(DV_LOCKALL_LEVEL);

	dv_printf("dv_pendsvtrap() after pushing preemption frame\n");
#if 0
	sysinfo();
	dumpPstack();
#endif
}

void dv_systickirq(void)
{
	dv_printf("dv_systickirq()\n");
	for (;;)	{ }
}

void dv_irq(void)
{

	dv_printf("dv_irq()\n");
#if 0
	sysinfo();
#endif

	/* Get the interrupt request
	*/
	int irq = ((int)dv_get_ipsr()) - 16;

	dv_printf(" - irq = %d\n", irq);
	
	if ( (irq < 0) || (irq >= DV_NVECTOR) )
		dv_panic(dv_panic_UnexpectedHardwareResponse, dv_sid_interruptdispatcher, "irq number out of range");

	/* Raise priorit yof current executable to highest task priority; save previous priority.
	 * This causes ISR executables to run immediately on the main (interrupt) stack, but any tasks
	 * that get woken up as a result get enqueued and run later on the appropriate thread/kernel stack.
	*/
	dv_id_t me = dv_currentexe;
	dv_prio_t my_p = dv_exe[me].currprio;
	if ( dv_exe[dv_currentexe].currprio < dv_maxtaskprio )
		dv_exe[dv_currentexe].currprio = dv_maxtaskprio;
	dv_printf(" - priority raised to %d\n", dv_maxtaskprio);

	/* Question: do we need to "boost" the level? Should be automatic.
	*/

#if DV_CFG_MAXEXTENDED > 0
	/* We're on the main stack, so inhibit switching to kernel stack to run ISRs.
	 * Only needed if there are extended tasks.
	*/
	extern dv_boolean_t dv_onkernelstack;
	dv_boolean_t my_oks = dv_onkernelstack;
	dv_onkernelstack = 1; 
#endif

	/* Call the vector function. If this activates an ISR executable, the ISR should run before
	 * returning.
	*/
	dv_softvector(irq);
	
	/* Sanity check
	*/
	if ( dv_currentexe != me )
		dv_panic(dv_panic_QueueCorrupt, dv_sid_interruptdispatcher, "current executable changed");

#if DV_CFG_MAXEXTENDED > 0
	/* Restore the kernel stack flag.
	 * Only needed if there are extended tasks.
	*/
	dv_onkernelstack = my_oks; 
#endif

	/* Restore priority of current executable
	*/
	dv_exe[me].currprio = my_p;

	/* Determine whether to trigger pendsv to perform context switch
	*/
	dv_prio_t p = dv_maxtaskprio;

	while ( p > my_p )
	{
		dv_id_t e = dv_qhead(p);

		if ( e > 0 )
		{
			/* Executable with higher priority than current executable found.
			 * Trigger PENDSV to run higher-priority tasks that the ISR woke up.
			*/
			dv_printf("dv_irq() - triggering PendSV\n");
			dv_m3scr.icsr = DV_PENDSVSET;
			break;
		}
		p--;
	}

	dv_printf("dv_irq() finished\n");
#if 0
	sysinfo();
#endif
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

