/*  dv-arm-bcm2835-gpio.c - UART on bcm2835 etc. (raspberry pi)
 *
 *  Copyright 2017 David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <devices/h/dv-arm-bcm2835-gpio.h>

/* dv_arm_bcm2835_gpio_pinconfig() - set up GPIO pin
 *
 * Select input, output or alternative functions
 * Set open, pull-down or pull-up.
*/
void dv_arm_bcm2835_gpio_pinconfig(dv_u32_t pin, dv_u32_t fsel, dv_u32_t pud)
{
	int index, shift;
	dv_u32_t mask, val;
	volatile int delay;

	index = pin/10;		/* fsel: 3 bits per field, 10 fields per register */
	shift = (pin%10) * 3;
	mask = 0x7 << shift;
	val = fsel << shift;

	dv_arm_bcm2835_gpio.fsel[index] = (dv_arm_bcm2835_gpio.fsel[index] & ~mask) | val;

	index = pin/32;		/* pudclk: 1 bit per field, 32 fields per register */
	shift = (pin%32);
	mask = 0x1 << shift;

	dv_arm_bcm2835_gpio.pud = pud;
	for ( delay = 0; delay < 150; delay++ )
	{
		/* Wait for 150 cycles setup time for the control signal */
	}
	dv_arm_bcm2835_gpio.pudclk[index] |= mask;
	for ( delay = 0; delay < 150; delay++ )
	{ 
		/* Wait for 150 cycles hold time for the control signal */
	}
	dv_arm_bcm2835_gpio.pudclk[index] &= ~mask;
	dv_arm_bcm2835_gpio.pud = 0;
}
