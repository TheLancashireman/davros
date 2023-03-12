/*  dv-stm32-usb.c
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
#include "dv-stm32-usb.h"
#include "dv-stm32-gpio.h"
#include "dv-stm32-rcc.h"

/* dv_stm32_usb_init() - initialise the usb controller
 *
 * Initialisation sequence (from sect. 23.4.2 of STM32F10x reference manual):
 *	- provide clock signals to USB controller (rcc?)
 *	- de-assert reset signal (rcc?)
 *
 * tinyusb does the rest.
 *
 * From data sheet:
 *	- Tstartup is Max 1 us !!!  So only have to wait 1us :-)
 *	- HSE aand PLL mustr be enabled, USBCLK = 48 MHz
 *	- USB pins USB_DP and USB_DM are  PA12 and PA11 resp. Automatically configured when USB enabled.
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
