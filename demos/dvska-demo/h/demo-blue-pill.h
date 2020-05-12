/* demo-blue-pill.h - configuration to allow the demo to run on a Blue Pill
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
#ifndef DEMO_BLUE_PILL_H
#define DEMO_BLUE_PILL_H	1

#include <dv-stm32-gpio.h>
#include <dv-stm32-uart.h>
#include <dv-nvic.h>

extern void sysinfo(void);

#define hw_UartInterruptId		0
#define hw_TimerInterruptId		0

static inline void hw_ClearTimer(void)
{
	/* FIXME: to do */
}

static inline void hw_SetLed(int i, dv_boolean_t state)
{
	/* FIXME: to do */
}

static inline void hw_EnableUartRxInterrupt(void)
{
	/* FIXME: to do */
}

static inline void hw_InitialiseMillisecondTicker(void)
{
	/* FIXME: to do */
}

#endif
