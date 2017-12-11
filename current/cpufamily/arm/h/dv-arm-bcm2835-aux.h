/*  dv-arm-bcm2835-aux.h - auxiliary peripherals ctrl/status on bcm2835 etc. (raspberry pi)
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
#ifndef dv_arm_bcm2835_aux_h
#define dv_arm_bcm2835_aux_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#if !DV_ASM

/* BCM2835 auxiliary peripherals (as used on Raspberry Pi CPUs).
 *
 * Two control/status registers for mini-uart and SPI peripherals.
*/
typedef struct dv_arm_bcm2835_aux_s dv_arm_bcm2835_aux_t;

struct dv_arm_bcm2835_aux_s
{
	dv_reg32_t irq;			/* 0x00	interrupt pending flags	(read-only) */
	dv_reg32_t enable;		/* 0x04	peripheral enable */
};

#define dv_arm_bcm2835_aux	((dv_arm_bcm2835_aux_t *)0x20215000)[0]

static inline void dv_arm_bcm2835_enable(dv_u32_t per)
{
	dv_arm_bcm2835_aux.enable |= per;
}

static inline void dv_arm_bcm2835_disable(dv_u32_t per)
{
	dv_arm_bcm2835_aux.enable &= ~per;
}

#endif

/* Masks for both registers.
*/
#define DV_AUX_uart	0x01
#define DV_AUX_spi1	0x02
#define DV_AUX_spi2	0x04

#endif
