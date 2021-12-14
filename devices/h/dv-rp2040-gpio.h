/*  dv-rp2040-gpio.h
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
#ifndef DV_RP2040_GPIO_H
#define DV_RP2040_GPIO_H

#include "dv-devices.h"
#include "dv-rp2040.h"

/* I/O user bank 0
 *
 * This peripheral controls the function selection for each GPIO pin.
*/
#define DV_IOBANK0_BASE	0x40014000

/* Pin status and control registers
*/
typedef struct dv_rp2040_gpio_sc_s
{
	volatile dv_u32_t	status;
	volatile dv_u32_t	ctrl;
} dv_rp2040_gpio_sc_t;

/* Interrupt control and status
*/
typedef struct dv_rp2040_io_intctl_s
{
	volatile dv_u32_t	inte[4];		/* 0x000 - interrupt enable */
	volatile dv_u32_t	intf[4];		/* 0x010 - interrupt force */
	volatile dv_u32_t	ints[4];		/* 0x020 - interrupt cooked status */
} dv_rp2040_io_intctl_t;

/* IOBANK0
*/
typedef struct dv_rp2040_iobank0_s
{
	dv_rp2040_gpio_sc_t		gpio[30];		/* 0x000 - GPIO 0..29 control and status */
	dv_rp2040_gpio_sc_t		gap[2];			/* Special space where GPIO 30..31 were left out ;-) */
	volatile dv_u32_t		intr[4];		/* 0x0f0 - raw interrupt status */
	dv_rp2040_io_intctl_t	proc_intctl[2];	/* 0x100 - interrupt control for each core */
	dv_rp2040_io_intctl_t	dormant_wake;	/* 0x160 - interrupt control for dormant wake */
} dv_rp2040_iobank0_t;

#define dv_rp2040_iobank0		(((dv_rp2040_iobank0_t *)(DV_IOBANK0_BASE))[0])
#define dv_rp2040_iobank0_xor	(((dv_rp2040_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_iobank0_w1s	(((dv_rp2040_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_iobank0_w1c	(((dv_rp2040_iobank0_t *)(DV_IOBANK0_BASE+DV_OFFSET_W1C))[0])

/* Bits 0..4 of gpio[n].ctrl select the function.
 *
 * The exact meaning (which peripheral, which signal) depends on the pin.
 * See section 2.19.2 in the RP2040 ref. manual for full details.
*/
#define DV_FUNCSEL_SPI		1
#define DV_FUNCSEL_UART		2
#define DV_FUNCSEL_I2C		3
#define DV_FUNCSEL_PWM		4
#define DV_FUNCSEL_SIO		5
#define DV_FUNCSEL_PIO0		6
#define DV_FUNCSEL_PIO1		7
#define DV_FUNCSEL_CLOCK	8
#define DV_FUNCSEL_USB		9
#define DV_FUNCSEL_NULL		31


/* Single-cycle I/O block.
 *
 * This is where the software control of the pins states take place.
 * The function selection for the pin has to be set to DV_FUNCSEL_SIO
 *
 * This block contains other features such as the core ID, inter-core FIFOs, spinlocks
 * and some arithmetical units.
*/
#define DV_SIO_BASE		0xd0000000

typedef struct dv_rp2040_gpioblk_s
{
	volatile dv_u32_t	val;
	volatile dv_u32_t	w1s;
	volatile dv_u32_t	w1c;
	volatile dv_u32_t	xor;
} dv_rp2040_gpioblk_t;

typedef struct dv_rp2040_interp_s
{
	volatile dv_u32_t accum0;				/* 0x00 - */
	volatile dv_u32_t accum1;				/* 0x04 - */
	volatile dv_u32_t base0;				/* 0x08 - */
	volatile dv_u32_t base1;				/* 0x0c - */
	volatile dv_u32_t base2;				/* 0x10 - */
	volatile dv_u32_t pop_lane0;			/* 0x14 - */
	volatile dv_u32_t pop_lane1;			/* 0x18 - */
	volatile dv_u32_t pop_full;				/* 0x1c - */
	volatile dv_u32_t peek_lane0;			/* 0x20 - */
	volatile dv_u32_t peek_lane1;			/* 0x24 - */
	volatile dv_u32_t peek_full;			/* 0x28 - */
	volatile dv_u32_t ctrl_lane0;			/* 0x2c - */
	volatile dv_u32_t ctrl_lane1;			/* 0x30 - */
	volatile dv_u32_t accum0_add;			/* 0x34 - */
	volatile dv_u32_t accum1_add;			/* 0x38 - */
	volatile dv_u32_t base_1and0;			/* 0x3c - */
} dv_rp2040_interp_t;

typedef struct dv_rp2040_sio_s
{
	volatile dv_u32_t	cpuid;				/* 0x000 - Core ID register */
	volatile dv_u32_t	gpio_in;			/* 0x004 - GPIO 0..29 input state */
	volatile dv_u32_t	gpio_hi_in;			/* 0x008 - QSPI GPIO input state */
	volatile dv_u32_t	gap1;
	dv_rp2040_gpioblk_t	gpio_out;			/* 0x010 - GPIO 0..29 output state */
	dv_rp2040_gpioblk_t	gpio_oe;			/* 0x020 - GPIO 0..29 output enable (1 = output, 0 = input) */
	dv_rp2040_gpioblk_t	gpio_hi_out;		/* 0x030 - QSPI GPIO output state */
	dv_rp2040_gpioblk_t	gpio_hi_oe;			/* 0x040 - QSPI GPIO output enable (1 = output, 0 = input) */
	volatile dv_u32_t	fifo_st;			/* 0x050 - FIFO status */
	volatile dv_u32_t	fifo_wr;			/* 0x054 - FIFO write */
	volatile dv_u32_t	fifo_rd;			/* 0x058 - FIFO read */
	volatile dv_u32_t	div_udividend;		/* 0x060 - unsigned dividend */
	volatile dv_u32_t	div_udivisor;		/* 0x064 - unsigned divisor */
	volatile dv_u32_t	div_sdividend;		/* 0x068 - signed dividend */
	volatile dv_u32_t	div_sdivisor;		/* 0x06c - signed divisor */
	volatile dv_u32_t	div_quotient;		/* 0x070 - quotient */
	volatile dv_u32_t	div_remainder;		/* 0x074 - remainder */
	volatile dv_u32_t	div_csr;			/* 0x078 - divider control/status */
	volatile dv_u32_t	gap2;
	dv_rp2040_interp_t	interp0;			/* 0x080 - interpolator 0 */
	dv_rp2040_interp_t	interp1;			/* 0x0c0 - interpolator 0 */
	volatile dv_u32_t	spinlock[32];		/* 0x100 - spinlocks */
} dv_rp2040_sio_t;

#define dv_rp2040_sio		(((dv_rp2040_sio_t *)DV_SIO_BASE)[0])

#endif
