/*	dv-arm-bcm2835-pcm.c - driver functions for the raspberry pi PCM/I2S interface
 *
 *	Copyright 2018 David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <dv-arm-bcm2835-pcm.h>
#include <dv-arm-bcm2835-gpio.h>

/* dv_pcm_init_i2s() - initialise for i2s operation
 *
 * This function is probably specific to the devices used.
 * ADC (input) is a PCM5102A, DAC is a PCM1808
 * PCM5102A is used in master mode to generate the clocks, because both ADC and DAC need
 * synchronised system, bit and LR clocks.
 * BCM2835 PCM device is used in slave mode.
*/
void dv_pcm_init_i2s(void)
{
	/* Disable the clock, set the mode register to a default state.
	*/
	dv_bcm2835_pcm.pcm_mode = DV_PCM_MODE_CLK_DIS;

	/* Set mode:
	 * CLK_DIS = 1 - clock still disabled (see above)
	 * CLKM = FSM = 1 - bit clock and frame clock as slave
	 * CLKI = FSI = 1 - bit and frame clocks inverted for i2s operation
	 * Frame len = FS len = 0 - these are for master mode only.
	*/
	dv_bcm2835_pcm.pcm_mode = DV_PCM_MODE_CLK_DIS | DV_PCM_MODE_CLKM | DV_PCM_MODE_FSM |
								DV_PCM_MODE_FSI | DV_PCM_MODE_CLKI;

	/* Set transmit config
	 * 32 bits (CH1WEX = CH2WEX = 1, CH1WID = CH2WID = 8)
	 * CH1 & CH2 enabled
	 * CH1 pos = 0, CH2 pos = 32
	 *
	 * The PCM1808 seems to be picky about this.
	*/
	dv_bcm2835_pcm.pcm_txc = DV_PCM_xXC_CH1WEX | DV_PCM_xXC_CH1EN | (0<<20) | (8<<16) |
							 DV_PCM_xXC_CH2WEX | DV_PCM_xXC_CH2EN | (32<<4) | (8<<0);

	/* Set receive config
	 * 24 bits (CH1WEX = CH2WEX = 1, CH1WID = CH2WID = 0)
	 * CH1 & CH2 enabled
	 * CH1 pos = 1, CH2 pos = 33
	 * This seems to be the correct setting for i2s.
	*/
	dv_bcm2835_pcm.pcm_rxc = DV_PCM_xXC_CH1WEX | DV_PCM_xXC_CH1EN | (1<<20) | (0<<16) |
							 DV_PCM_xXC_CH2WEX | DV_PCM_xXC_CH2EN | (33<<4) | (0<<0);

	/* Enable te clock
	*/
	dv_bcm2835_pcm.pcm_mode &= ~DV_PCM_MODE_CLK_DIS;

	/* Enable device, Tx and Rx on, sign-extend RX
	*/
	dv_bcm2835_pcm.pcm_cs |= DV_PCM_CS_EN | DV_PCM_CS_TXON | DV_PCM_CS_RXON | DV_PCM_CS_RXSEX;

	/* Select the pins for I2S
	 * GPIO18 Alt0 = PCM_CLK
	 * GPIO19 Alt0 = PCM_FS
	 * GPIO20 Alt0 = PCM_DIN
	 * GPIO21 Alt0 = PCM_DOUT
	 * If the I2S interface is in slave mode, it's best to set the pin modes after
	 * configuring the unit.
	*/
	dv_arm_bcm2835_gpio_pinconfig(18, DV_pinfunc_alt0, DV_pinpull_up);
	dv_arm_bcm2835_gpio_pinconfig(19, DV_pinfunc_alt0, DV_pinpull_up);
	dv_arm_bcm2835_gpio_pinconfig(20, DV_pinfunc_alt0, DV_pinpull_up);
	dv_arm_bcm2835_gpio_pinconfig(21, DV_pinfunc_alt0, DV_pinpull_up);
}
