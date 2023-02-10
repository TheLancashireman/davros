/*  dv-stm32-gpio.c
 *
 *  Copyright David Haworth
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
#include "dv-stm32-gpio.h"
#include "dv-stm32-rcc.h"

/* dv_stm32_gpio_pinmode() - select the mode of a GPIO pin
 *
 * Parameters:
 *	- iop	:	'a', 'b', etc.
 *	- pin	:	pin number (0..15)
 *	- mode	:	
*/
void dv_stm32_gpio_pinmode(char iop, int pin, dv_u32_t mode)
{
	dv_gpio_t *gpio;
	
	switch ( iop )
	{
	case 'a':
		gpio = &dv_gpio_a;
		dv_rcc.apb2en |= DV_RCC_IOPA;
		break;

	case 'b':
		gpio = &dv_gpio_b;
		dv_rcc.apb2en |= DV_RCC_IOPB;
		break;

	case 'c':
		gpio = &dv_gpio_c;
		dv_rcc.apb2en |= DV_RCC_IOPC;
		break;

	default:
		return;
	}
	
    int cr = pin / 8;
    int shift = (pin % 8) * 4;
    dv_u32_t mask = 0xf << shift;
    dv_u32_t val = mode << shift;
    gpio->cr[cr] = (gpio->cr[cr] & ~mask) | val;
}
