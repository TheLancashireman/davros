/ demo-blue-pill.h - configuration to allow the demo to run on a Blue Pill
 *
 * (c) 2020 David Haworth
*/
#ifndef DEMO_BLUE_PILL_H
#define DEMO_BLUE_PILL_H	1

#include <dv-stm32-gpio.h>
#include <dv-stm32-uart.h>
#include <dv-nvic.h>

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
