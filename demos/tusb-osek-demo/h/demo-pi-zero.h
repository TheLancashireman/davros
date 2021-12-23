/* demo-pi-zero.h - configuration to allow the demo to run on a raspberry pi zero
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
#ifndef DEMO_PI_ZERO_H
#define DEMO_PI_ZERO_H	1

#include <dv-arm-bcm2835-gpio.h>
#include <dv-arm-bcm2835-uart.h>
#include <dv-arm-bcm2835-aux.h>
#include <dv-arm-bcm2835-interruptcontroller.h>
#include <dv-armv6-mmu.h>
#include <dv-arm-cp15.h>
#include <dv-arm-bcm2835-systimer.h>
#include <dv-arm-bcm2835-armtimer.h>

#define hw_UartInterruptId		dv_iid_aux
#define hw_TimerInterruptId		dv_iid_timer

static inline void hw_ClearTimer(void)
{
	dv_arm_bcm2835_armtimer_clr_irq();		/* Clear the interrupt */
}

static inline void hw_SetLed(int i, dv_boolean_t state)
{
	if ( (i < 0) || (i >= 4) )	return;

	static const dv_u32_t led_map[4] = {17, 18, 27, 22};
	if ( state )
		dv_arm_bcm2835_gpio_pin_clear(led_map[i]);
	else
		dv_arm_bcm2835_gpio_pin_set(led_map[i]);
}

static inline void hw_EnableUartRxInterrupt(void)
{
	dv_arm_bcm2835_uart.ier |= DV_IER_RxInt;
}

static inline void hw_InitialiseMillisecondTicker(void)
{
	dv_arm_bcm2835_armtimer_init(1);			/* Use a prescaler of 1 for high resolution */
	dv_arm_bcm2835_armtimer_set_load(250000);
}

#endif
