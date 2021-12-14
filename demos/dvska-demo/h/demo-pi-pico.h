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
#include <dv-nvic.h>

#define LED_PIN	25	/* On-board LED */

extern void sysinfo(void);
extern void dumpPstack(void);

#define panic_info() \
	do {				\
		sysinfo();		\
		dumpPstack();	\
	} while (0)

#define hw_UartInterruptId		dv_irq_uart0
#define hw_TimerInterruptId		dv_irq_timer0

static inline void hw_ClearTimer(void)
{
#if 0	/* TODO */
	dv_tim2.ccr[3] += 1000;			/* No in-the-past check */
	dv_tim2.sr &= ~DV_TIM_CC4IF;	/* Clear the match flag */
#endif
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
#if 0	/* TODO */
	dv_uart1.cr[0] |= DV_UART_RXNEIE;
	dv_enable_irq(dv_irq_usart1);
#endif
}

static inline void hw_InitialiseMillisecondTicker(void)
{
#if 0	/* TODO */
	/* Set the counter running with a 1 us tick
	*/
	dv_stm32_timx_init(2, 72);

	/* Enable CC4 interrupt
	*/
	dv_tim2.dier |= DV_TIM_CC4IE;

	/* Set the compare mode:
	 *	- CC4S = 0	--> compare mode
	 *	- OC4CE = 0	--> ETRF has no effect
	 *	- OC4M = 0	--> compare has no effect on output
	 *	- OC4PE = 0	--> CCR4 is not buffered, can be written any time
	 *	- OC4FE = 0	--> normal trigger effect
	*/
	dv_tim2.ccmr[1] = (dv_tim2.ccmr[1] & 0xff) | ( (0) << 8 );

	/* Set the compare register: initial delay of 1 ms
	*/
	dv_tim2.ccr[3] = dv_tim2.cnt + 1000;

	/* Enable the comparison operation
	 *	- clear the match flag
	 *	- enable event generation
	 *	- enable the compare function
	*/
	dv_tim2.sr &= ~DV_TIM_CC4IF;
	dv_tim2.egr |= DV_TIM_CC4G;
	dv_tim2.ccer |= DV_TIM_CCxE << 12;
#endif
}

#endif
