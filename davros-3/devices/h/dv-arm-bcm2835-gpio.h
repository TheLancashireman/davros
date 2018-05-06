/*  dv-arm-bcm2835-gpio.h - UART on bcm2835 etc. (raspberry pi)
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
#ifndef dv_arm_bcm2835_gpio_h
#define dv_arm_bcm2835_gpio_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#if !DV_ASM

/* There are 54 GPIO outputs, most of which have at least one more function
 * on top of the standard digital I/O.
 *
 * resn[] fields are reserved (unused) locations in the memory map.
 *
 * For the fields that have 1 bit per pin:
 *		xxx[0] 31..0 represents gpio 31..0
 *		xxx[1] 21..0 represents gpio 53..32
 *      (actually, in the doc this is reversed: bits 31..0 are gpio 0..31 and 21..0 are gpio 32..53)
 *
 * The function select registers have 3 bits per pin (10 pins per register)
 *		E.g. in register 0, pin 0 is bits 0..2 ---> pin 9 is bits 27..29. Bits 30..31 not used.
 *		For each pin: 000 = input, 001 = output, other values are alternate functions.
 *
 * pud register: lower 2 bits:
 *		00 = pull up/down disabled
 *		01 = pull down enabled
 *		10 = pull up enabled
 *		11 reserved
 *
 * pudclk registers ---> looks complicated. Might be easier to use external Rs :-)
*/

/* Function select - yes, they really are in that order!
*/
#define DV_pinfunc_input	0
#define DV_pinfunc_output	1
#define DV_pinfunc_alt0		4
#define DV_pinfunc_alt1		5
#define DV_pinfunc_alt2		6
#define DV_pinfunc_alt3		7
#define DV_pinfunc_alt4		3
#define DV_pinfunc_alt5		2

/* Pull up/down
*/
#define DV_pinpull_none		0
#define DV_pinpull_down		1
#define DV_pinpull_up		2

typedef struct dv_arm_bcm2835_gpio_s dv_arm_bcm2835_gpio_t;

struct dv_arm_bcm2835_gpio_s
{
	dv_reg32_t fsel[6];		/* 3 bits per pin ==> 10 pins per register. Bits 30..31 not used. */
	dv_reg32_t res1[1];
	dv_reg32_t set[2];		/* write 1 to set (32 pins per register) */
	dv_reg32_t res2[1];
	dv_reg32_t clr[2];		/* write 1 to clear (32 pins per register) */
	dv_reg32_t res3[1];
	dv_reg32_t lev[2];		/* pin level registers */
	dv_reg32_t res4[1];
	dv_reg32_t eds[2];		/* event detect registers */
	dv_reg32_t res5[1];
	dv_reg32_t ren[2];		/* rising edge detect enable registers */
	dv_reg32_t res6[1];
	dv_reg32_t fen[2];		/* falling edge detect enable registers */
	dv_reg32_t res7[1];
	dv_reg32_t hen[2];		/* high detect enable registers */
	dv_reg32_t res8[1];
	dv_reg32_t len[2];		/* low detect enable registers */
	dv_reg32_t res9[1];
	dv_reg32_t aren[2];		/* async rising edge detect enable registers */
	dv_reg32_t res10[1];
	dv_reg32_t afen[2];		/* async falling edge detect enable registers */
	dv_reg32_t res11[1];
	dv_reg32_t pud;			/* Pull up/down enable */
	dv_reg32_t pudclk[2];	/* Pull up/down enable clock */
};

#define dv_arm_bcm2835_gpio	((dv_arm_bcm2835_gpio_t *)0x20200000)[0]

void dv_arm_bcm2835_gpio_pinconfig(dv_u32_t pin, dv_u32_t fsel, dv_u32_t pud);

static inline void dv_arm_bcm2835_gpio_pin_set(dv_u32_t pin)
{
	dv_arm_bcm2835_gpio.set[pin/32] = 1 << (pin%32);
}

static inline void dv_arm_bcm2835_gpio_pin_clear(dv_u32_t pin)
{
	dv_arm_bcm2835_gpio.clr[pin/32] = 1 << (pin%32);
}

static inline void dv_arm_bcm2835_gpio_pin_set_group(dv_u64_t group)
{
	dv_arm_bcm2835_gpio.set[0] = group & 0xffffffff;
	dv_arm_bcm2835_gpio.set[1] = group >> 32;
}

static inline void dv_arm_bcm2835_gpio_pin_clear_group(dv_u64_t group)
{
	dv_arm_bcm2835_gpio.clr[0] = group & 0xffffffff;
	dv_arm_bcm2835_gpio.clr[1] = group >> 32;
}

#endif

#endif
