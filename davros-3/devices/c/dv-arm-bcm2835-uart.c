/*  dv-arm-bcm2835-uart.c - UART on bcm2835 etc. (raspberry pi)
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
#include <cpufamily/arm/h/dv-arm-bcm2835-uart.h>
#include <cpufamily/arm/h/dv-arm-bcm2835-aux.h>
#include <cpufamily/arm/h/dv-arm-bcm2835-gpio.h>
#include <kernel/h/dv-stdio.h>

static int dv_arm_bcm2835_uart_putc(int c);
static int dv_arm_bcm2835_uart_getc(void);
static int dv_arm_bcm2835_uart_istx(void);
static int dv_arm_bcm2835_uart_isrx(void);

void dv_arm_bcm2835_uart_init(dv_u32_t baud, dv_u32_t bits, dv_u32_t parity)
{
	if ( baud == 115200 &&			/* ToDo: Other bitrates */
		 (bits == 7 || bits == 8) &&
		 (parity == 0 ) )
	{
		dv_arm_bcm2835_enable(DV_AUX_uart);

		dv_arm_bcm2835_uart.cntl = 0;		/* Tx and Rx disabled */
		dv_arm_bcm2835_uart.ier = 0;		/* Interrupts disabled */
		dv_arm_bcm2835_uart.lcr = (bits==7 ? DV_LCR_7bit : DV_LCR_8bit);
		dv_arm_bcm2835_uart.mcr = 0;		/* RTS high (not used) */
		dv_arm_bcm2835_uart.baud = 270;		/* 115200 */

		dv_arm_bcm2835_gpio_pinconfig(14, DV_pinfunc_alt5, DV_pinpull_none);	/* Transmit pin gpio14 */
		dv_arm_bcm2835_gpio_pinconfig(15, DV_pinfunc_alt5, DV_pinpull_none);	/* Receive pin gpio15 */

	    dv_arm_bcm2835_uart.cntl = DV_CNTL_TxEn | DV_CNTL_RxEn;
	}
	else
	{
		dv_arm_bcm2835_disable(DV_AUX_uart);
	}
}

void dv_arm_bcm2835_uart_console(void)
{
	dv_consoledriver.putc = dv_arm_bcm2835_uart_putc;
	dv_consoledriver.getc = dv_arm_bcm2835_uart_getc;
	dv_consoledriver.istx = dv_arm_bcm2835_uart_istx;
	dv_consoledriver.isrx = dv_arm_bcm2835_uart_isrx;
}

static int dv_arm_bcm2835_uart_putc(int c)
{
	while ( !dv_arm_bcm2835_uart_istx() )
	{
		/* Wait till there's room */
	}
	dv_arm_bcm2835_uart.io = c;
	return 1;
}

static int dv_arm_bcm2835_uart_getc(void)
{
	while ( !dv_arm_bcm2835_uart_istx() )
	{
		/* Wait till there's a character */
	}
	return (int)dv_arm_bcm2835_uart.io;
}

static int dv_arm_bcm2835_uart_istx(void)
{
	return ( (dv_arm_bcm2835_uart.stat & DV_STAT_TxSpace) != 0 );
}

static int dv_arm_bcm2835_uart_isrx(void)
{
	return ( (dv_arm_bcm2835_uart.stat & DV_STAT_RxNchars) != 0 );
}
