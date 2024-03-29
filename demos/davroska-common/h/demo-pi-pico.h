/* demo-pi-pico.h - configuration to allow the demo to run on a Raspberry Pi Pico
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
#ifndef DEMO_PI_PICO_H
#define DEMO_PI_PICO_H	1

#include <dv-rp2040-gpio.h>
#include <dv-rp2040-uart.h>
#include <dv-rp2040-interrupts.h>
#include <dv-rp2040-timer.h>
#include <dv-nvic.h>
#include <dv-stdio.h>

#define LED_PIN	25	/* On-board LED */

#define ALARM_IDX	0

extern void sysinfo(void);
extern void dumpPstack(void);

#define panic_info() \
	do {				\
		sysinfo();		\
		dumpPstack();	\
	} while (0)

#define hw_UartInterruptId		dv_irq_uart0
#define hw_TimerInterruptId		dv_irq_timer0
#define hw_UsbInterruptId1		dv_irq_usbctrl

#if 1
#define hw_UsbIsr1()			dcd_int_handler(0)
#else
extern void (*tusb_isr_func)(void);

/* ISR body.
*/
static inline void hw_UsbIsr1(void)
{
	if ( tusb_isr_func == 0 )
	{
		dv_printf("hw_UsbIsr1() called before handler configured\n");
	}
	else
	{
		(*tusb_isr_func)();
	}
}
#endif

/* Let tinyusb handle enabling and disabling the interrupt
*/
#define	hw_EnableUsbIrqs()	do {} while (0)

static inline void hw_ClearTimer(void)
{
	dv_rp2040_timer.intr = 0x1 << ALARM_IDX;
	dv_rp2040_timer.alarm[ALARM_IDX] += 1000;	/* No in-the-past check */
}

static inline void hw_SetLed(int i, dv_boolean_t state)
{
	if ( i == 3 )
	{
		if ( state )
			dv_rp2040_sio.gpio_out.w1s = 0x1 << LED_PIN;
		else
			dv_rp2040_sio.gpio_out.w1c = 0x1 << LED_PIN;
	}
}

static inline void hw_EnableUartRxInterrupt(void)
{
	/* Get an interrupt when:
	 * 		- fifo reaches watermark (default: half full), or
	 *		- fifo is not empty and no characters received for 32 bit times
	*/
	dv_rp2040_uart0_w1s.imsc = (DV_UART_RXIM | DV_UART_RTIM);
}

static inline void hw_InitialiseMillisecondTicker(void)
{
	/* Enable the alarm interrupt in the counter
	*/
	dv_rp2040_timer_w1s.inte = 0x1 << ALARM_IDX;
	dv_enable_irq(dv_irq_timer0+ALARM_IDX);

	/* Set and arm the alarm
	*/
	dv_rp2040_timer.alarm[ALARM_IDX] = dv_rp2040_timer.time_lraw + 1000000;
}

#endif
