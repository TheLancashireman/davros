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
#include <dv-stm32-gpio.h>

#include <davroska-inline.h>

#include <demo-blue-pill.h>

#define DV_SPSEL		0x02

extern unsigned dv_start_data, dv_end_data, dv_start_bss, dv_end_bss, dv_idata;
extern unsigned dv_pstacktop;
extern unsigned dv_stacktop;

extern int main(int argc, char **argv);
extern void dv_switchToPsp(unsigned *psp, unsigned *msp, unsigned control, void (*fp)(void));

extern void dv_stm32_usb_init(void);

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

	/* Initialize the USB controller and its clocks
	*/
	dv_stm32_usb_init();

	/* Initialise variables
	*/
	dv_init_data();

	/* Initialise the exception priorities
	*/
	dv_ctxm_scr.shpr[0] = 0x0;			/* UsageFault, BusFault, MemManage and [reserved]  all at highest priority */
	dv_ctxm_scr.shpr[1] = 0x0;			/* SVC and [reserved x 3] all at highest priority */
	dv_ctxm_scr.shpr[2] = 0xffff0000;	/* SysTick/PendSV at lowest priority, Debug and [reserved] at highest */

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

	/* Initialise GPIO C for the on-board LED
	 *
	 * Pin 13 (LED_PIN) output, open-drain, 50 MHz, output to 1 (turn off)
	*/
	do {
		dv_rcc.apb2en |= DV_RCC_IOPC;
		int cr = LED_PIN / 8;
		int shift = (LED_PIN % 8) * 4;
		dv_u32_t mask = 0xf << shift;
		dv_u32_t val = DV_GPIO_OUT_OD_50 << shift;
		dv_gpio_c.cr[cr] = (dv_gpio_c.cr[cr] & mask) | val;
		dv_gpio_c.bsrr = 0x1 << LED_PIN;
	} while (0);

	/* Print the contents of the MPU_TYPE register
	*/
	dv_printf("MPU_TYPE = 0x%08x\n", *(unsigned *)0xE000ED90);

	sysinfo();

	/* It would be possible to pass main() as the function pointer here,
	 * but for the time being we'll use an intermediate function so that we can find out
	 * what's going on.
	*/
	dv_switchToPsp(&dv_pstacktop, &dv_stacktop, (dv_get_control() | DV_SPSEL), &dv_reset2);
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

void dv_unknowntrap(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! An undocumented trap occurred");
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

void dumpPstack(void)
{
	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();

	for ( int i=15; i>=0; i-- )
	{
		dv_printf("psp[%02d] (0x%08x) = 0x%08x\n", i, (dv_u32_t)&psp[i], psp[i]);
	}
}

/* dv_stm32_usb_init() - initialise the usb controller
 *
 * See also dv_stm32_usb_connect()
 *
 * Initialisation sequence (from sect. 23.4.2 of STM32F10x reference manual):
 *  - provide clock signals to USB controller (rcc?)
 *  - de-assert reset signal (rcc?)
 *  - turn on analog circuitry (de-assert PDWN)
 *  - wait Tstartup (see data sheet)
 *  - de-assert controller reset (FRES)
 *  - clear spurious interrupts (ISTR)
 *  - initialise the packet buffer table (probably before removing PDWN?)
 *
 * From data sheet:
 *  - Tstartup is Max 1 us !!!  So only have to wait 1us :-)
 *  - HSE aand PLL mustr be enabled, USBCLK = 48 MHz
 *  - USB pins USB_DP and USB_DM are  PA12 and PA11 resp. Automatically configured when USB enabled.
*/
void dv_stm32_usb_init(void)
{
	/* Assert the reset signal
	*/
	dv_rcc.apb1rst |= DV_RCC_USB;

	/* Ensure a 1.5 prescaler to divide the 72 MHz down to 48 MHz for the USB port
	*/
	dv_rcc.cfg &= ~DV_RCC_USBPRE;

	/* Enable the clock to the USB peripheral
	*/
	dv_rcc.apb1en |= DV_RCC_USB;

	/* Deassert the reset signal
	*/
	dv_rcc.apb1rst &= ~DV_RCC_USB;
}
