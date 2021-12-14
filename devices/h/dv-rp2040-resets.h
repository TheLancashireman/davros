/*  dv-rp2040-resets.h
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
#ifndef DV_RP2040_RESETS_H
#define DV_RP2040_RESETS_H

#include "dv-devices.h"
#include "dv-rp2040.h"

/* The resets registers control the power to the peripherals.
 * A peripheral whose reset bit is 1 is held in reset.
*/
#define DV_RESETS_BASE	0x4000c000

typedef struct dv_rp2040_resets_s
{
	volatile dv_u32_t	reset;
	volatile dv_u32_t	wdsel;
	volatile dv_u32_t	done;
} dv_rp2040_resets_t;

#define dv_rp2040_resets		(((dv_rp2040_resets_t *)DV_RESETS_BASE)[0])
#define dv_rp2040_resets_xor	(((dv_rp2040_resets_t *)(DV_RESETS_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_resets_w1s	(((dv_rp2040_resets_t *)(DV_RESETS_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_resets_w1c	(((dv_rp2040_resets_t *)(DV_RESETS_BASE+DV_OFFSET_W1C))[0])

/* Bits in the RESETS registers
*/
#define DV_RESETS_usbctrl		0x01000000
#define DV_RESETS_uart1			0x00800000
#define DV_RESETS_uart0			0x00400000
#define DV_RESETS_timer			0x00200000
#define DV_RESETS_tbman			0x00100000
#define DV_RESETS_sysinfo		0x00080000
#define DV_RESETS_syscfg		0x00040000
#define DV_RESETS_spi1			0x00020000
#define DV_RESETS_spi0			0x00010000
#define DV_RESETS_rtc			0x00008000
#define DV_RESETS_pwm			0x00004000
#define DV_RESETS_pll_usb		0x00002000
#define DV_RESETS_pll_sys		0x00001000
#define DV_RESETS_pio1			0x00000800
#define DV_RESETS_pio0			0x00000400
#define DV_RESETS_pads_qspi		0x00000200
#define DV_RESETS_pads_bank0	0x00000100
#define DV_RESETS_jtag			0x00000080
#define DV_RESETS_io_qspi		0x00000040
#define DV_RESETS_io_bank0		0x00000020
#define DV_RESETS_i2c1			0x00000010
#define DV_RESETS_i2c0			0x00000008
#define DV_RESETS_dma			0x00000004
#define DV_RESETS_busctrl		0x00000002
#define DV_RESETS_adc			0x00000001

/* dv_rp2040_release() - bring a peripheral out of reset
 *
 * The parameter must specify exactly one peripheral
*/
static inline void dv_rp2040_release(dv_u32_t peri)
{
	if ( (dv_rp2040_resets.reset & peri) != 0 )		/* Only do this if the device is in reset */
	{
		dv_rp2040_resets_w1c.reset = peri;
    	do {    /* Wait */  } while ( (dv_rp2040_resets.done & peri) == 0 );
	}
}

/* dv_rp2040_reset() - reset a peripheral
 *
 * The parameter must specify exactly one peripheral
*/
static inline void dv_rp2040_reset(dv_u32_t peri)
{
	dv_rp2040_resets_w1s.reset = peri;				/* Put the peripheral into reset */
	dv_rp2040_release(peri);						/* Bring it back out again */
}

#endif
