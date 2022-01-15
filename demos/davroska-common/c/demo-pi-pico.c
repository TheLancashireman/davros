/* demo-pi-pico.c - hardware-specific functions for davroska demo
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

#include <dv-rp2040-clocks.h>
#include <dv-rp2040-resets.h>
#include <dv-nvic.h>
#include <dv-rp2040-uart.h>
#include <dv-rp2040-usb.h>
#include <dv-rp2040-gpio.h>
#include <dv-rp2040-timer.h>

#include <davroska-inline.h>

#include <demo-pi-pico.h>

#define DV_SPSEL		0x02

extern unsigned dv_start_data, dv_end_data, dv_start_bss, dv_end_bss, dv_idata;
extern unsigned dv_pstacktop;
extern unsigned dv_stacktop;

extern int main(int argc, char **argv);
extern void dv_switchToPsp(unsigned *psp, unsigned *msp, unsigned control, void (*fp)(void));

/* LED debugging
*/
void softdelay(unsigned ms)
{
	for ( volatile unsigned dly = 0; dly < 10000 * ms; dly++ )
	{
	}
}
void blink(int n)
{
	softdelay(3000);

	for (int i = 0; i < n; i++)
	{
		hw_SetLed(3, 1);
		softdelay(50);
		hw_SetLed(3, 0);
		softdelay(950);
	}
}


/* dv_init_data() - initialise variables
 *
 * Initialises all variables from the flash image (.data) or to zero (.bss)
*/
void dv_init_data(void)
{
#if 0
	unsigned *s = &dv_idata;
	unsigned *d = &dv_start_data;

	while ( d < &dv_end_data )
	{
		*d++ = *s++;
	}
#endif

	unsigned *b = &dv_start_bss;
	while ( b < &dv_end_bss )
	{
		*b++ = 0x00;
	}
}

/* Mapping functions for console
*/
int uart0_putc(int c)
{
	dv_rp2040_uart_putc(&dv_rp2040_uart0, c);
	return 1;
}

int uart0_getc(void)
{
	return dv_rp2040_uart_getc(&dv_rp2040_uart0);
}

int uart0_isrx(void)
{
	return dv_rp2040_uart_isrx(&dv_rp2040_uart0);
}

int uart0_istx(void)
{
	return dv_rp2040_uart_istx(&dv_rp2040_uart0);
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
	/* Set up the vector table
    */
    dv_ctxm_scr.vtor = (dv_u32_t)&dv_hwvectors[0];

	/* Initialise the the XOSC clock and the PLL: 133 MHz
	*/
	dv_clock_init();
	dv_pll_init();
#if USE_USB
	dv_usbpll_init();
#endif


	/* Initialise variables
	*/
	dv_init_data();

	/* Initialise the exception priorities
	*/
	dv_ctxm_scr.shpr[1] = 0x0;			/* SVC and [reserved x 3] all at highest priority */
	dv_ctxm_scr.shpr[2] = 0xffff0000;	/* SysTick/PendSV at lowest priority, Debug and [reserved] at highest */

	/* Release IOBANK0 from reset.
	 * We must do this before any pin functions can be selected.
	*/
	dv_rp2040_release(DV_RESETS_io_bank0);

	/* Initialise GPIO pin for the on-board LED
	*/
	dv_sio_pin_init(LED_PIN, 1);
	hw_SetLed(3, 0);

	/* Initialise uart0 and connect it to the stdio functions
	*/
	(void)dv_rp2040_uart_init(&dv_rp2040_uart0, 115200, "8N1");

	/* Set up the I/O function for UART0
     * GPIO 0 = UART0 tx
     * GPIO 1 = UART0 rx
    */
    dv_rp2040_iobank0.gpio[0].ctrl = DV_FUNCSEL_UART;
    dv_rp2040_iobank0.gpio[1].ctrl = DV_FUNCSEL_UART;

	dv_consoledriver.putc = uart0_putc;
	dv_consoledriver.getc = uart0_getc;
	dv_consoledriver.istx = uart0_istx;
	dv_consoledriver.isrx = uart0_isrx;

	dv_printf("\nHello, galaxy! RP2040 calling\n");

	/* Release the timer from reset
	*/
	dv_rp2040_release(DV_RESETS_timer);
	
	/* Initialise the interrupt controller
	*/
	dv_nvic_init();

	/* Print the contents of the MPU_TYPE register
	*/
	dv_printf("MPU_TYPE = 0x%08x\n", *(unsigned *)0xE000ED90);

#if USE_USB
	/* Initialise the USB controller
	*/
	(void)dv_rp2040_usb_init();
#endif

	sysinfo();

#if 0
	/* TEMPORARY: switch timer clock to USB PLL to check speed
	*/
	dv_rp2040_clocks.ref.ctrl = DV_CLKSRC_REF_AUX; 	/* Aux_src = 0 ==> usb_pll */
#endif

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

#if USE_USB
/* pico-sdk gasket for tinyusb
*/
void (*tusb_isr_func)(void);

void irq_set_exclusive_handler(dv_irqid_t irq, void (*fn)(void))
{
	if ( irq == hw_UsbInterruptId1 )
	{
		dv_printf("irq_set_exclusive_handler() called : setting handler to 0x%08x\n", (unsigned)fn);
		tusb_isr_func = fn;
	}
	else
	{
		dv_printf("irq_set_exclusive_handler() called : irq != dv_irq_usbctrl\n");
	}
}

void irq_set_enabled(dv_irqid_t irq, int en)
{
	if ( irq == hw_UsbInterruptId1 )
	{
		if ( en )
		{
			dv_printf("irq_set_enabled() called : enabling USB IRQ\n");
			dv_enable_irq(hw_UsbInterruptId1);
		}
		else
		{
			dv_printf("irq_set_enabled() called : disabling USB IRQ\n");
			dv_disable_irq(hw_UsbInterruptId1);
		}
	}
	else
	{
		dv_printf("irq_set_enabled() called : irq != dv_irq_usbctrl\n");
	}
}
#endif
