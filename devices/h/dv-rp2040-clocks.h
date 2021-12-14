/*  dv-rp2040-clocks.h
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
#ifndef DV_RP2040_CLOCKS_H
#define DV_RP2040_CLOCKS_H

#include "dv-devices.h"
#include "dv-rp2040.h"

/* Clocks
*/
#define DV_CLOCKS_BASE	0x40008000

typedef struct dv_rp2040_clkblk_s
{
	volatile dv_u32_t ctrl;
	volatile dv_u32_t div;
	volatile dv_u32_t selected;
} dv_rp2040_clkblk_t;

typedef struct dv_rp2040_clocks_s
{
	dv_rp2040_clkblk_t	gpout[4];			/* 0x00/0x0c/0x18/0x24 */
	dv_rp2040_clkblk_t	ref;				/* 0x30 - */
	dv_rp2040_clkblk_t	sys;				/* 0x3c - */
	dv_rp2040_clkblk_t	peri;				/* 0x48 - no DIV in this block? */
	dv_rp2040_clkblk_t	usb;				/* 0x54 - */
	dv_rp2040_clkblk_t	adc;				/* 0x60 - */
	dv_rp2040_clkblk_t	rtc;				/* 0x6c - */
	volatile dv_u32_t	sys_resus_ctrl;		/* 0x74 - */
	volatile dv_u32_t	sys_resus_status;	/* 0x7c - */
	volatile dv_u32_t	fc0_ref_khz;		/* 0x80 - */
	volatile dv_u32_t	fc0_min_khz;		/* 0x84 - */
	volatile dv_u32_t	fc0_max_khz;		/* 0x88 - */
	volatile dv_u32_t	fc0_delay;			/* 0x8c - */
	volatile dv_u32_t	fc0_interval;		/* 0x90 - */
	volatile dv_u32_t	fc0_src;			/* 0x94 - */
	volatile dv_u32_t	fc0_status;			/* 0x98 - */
	volatile dv_u32_t	fc0_result;			/* 0x9c - */
	volatile dv_u32_t	wake_en[2];			/* 0xa0 - */
	volatile dv_u32_t	sleep_en[2];		/* 0xa8 - */
	volatile dv_u32_t	enabled[2];			/* 0xb0 - */
	volatile dv_u32_t	intr;				/* 0xb8 - */
	volatile dv_u32_t	inte;				/* 0xbc - */
	volatile dv_u32_t	intf;				/* 0xc0 - */
	volatile dv_u32_t	ints;				/* 0xc4 - */
} dv_rp2040_clocks_t;

#define dv_rp2040_clocks		(((dv_rp2040_clocks_t *)(DV_CLOCKS_BASE))[0])
#define dv_rp2040_clocks_xor	(((dv_rp2040_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_clocks_w1s	(((dv_rp2040_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_clocks_w1c	(((dv_rp2040_clocks_t *)(DV_CLOCKS_BASE+DV_OFFSET_W1C))[0])

#define DV_CLK_ENABLE	0x00000800
#define DV_CLK_KILL		0x00000400

#define DV_CLKSRC_REF_ROSC		0x00
#define DV_CLKSRC_REF_AUX		0x01
#define DV_CLKSRC_REF_XOSC		0x02
#define DV_CLKSRC_SYS_REF		0x00
#define DV_CLKSRC_SYS_AUX		0x01
#define DV_CLKSRC_SYS_AUX_PLL	0x00
#define DV_CLKSRC_SYS_AUX_UPLL	0x20
#define DV_CLKSRC_SYS_AUX_ROSC	0x40
#define DV_CLKSRC_SYS_AUX_XOSC	0x60
#define DV_CLKSRC_PERI_SYS		0x00
#define DV_CLKSRC_PERI_PLL		0x20
#define DV_CLKSRC_PERI_UPLL		0x40
#define DV_CLKSRC_PERI_ROSC		0x60
#define DV_CLKSRC_PERI_XOSC		0x80

/* Xosc
*/
#define DV_XOSC_BASE			0x40024000

typedef struct dv_rp2040_xosc_s
{
	volatile dv_u32_t	ctrl;
	volatile dv_u32_t	status;
	volatile dv_u32_t	dormant;
	volatile dv_u32_t	startup;
	volatile dv_u32_t	count;
} dv_rp2040_xosc_t;

#define dv_rp2040_xosc		(((dv_rp2040_xosc_t *)(DV_XOSC_BASE))[0])
#define dv_rp2040_xosc_xor	(((dv_rp2040_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_xosc_w1s	(((dv_rp2040_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_xosc_w1c	(((dv_rp2040_xosc_t *)(DV_XOSC_BASE+DV_OFFSET_W1C))[0])

#define DV_XOSC_1_15_MHZ	0x00000aa0
#define DV_XOSC_ENABLE		0x00fab000
#define DV_XOSC_DISABLE		0x00d1e000
#define DV_XOSC_STARTUP_X4	0x00100000
#define DV_XOSC_STABLE		0x80000000
#define DV_XOSC_BADWRITE	0x01000000
#define DV_XOSC_ENABLED		0x00001000

/* PLL
 * The sys PLL can be used to drive the cores. For full speed we need to wind it up to 133 MHz
 *
 * From the block diagram and description in refman 2.18:
 *	REFDIV = 1 because FREF is 12 MHz.
 *	VCO output must be in range 400..1600 MHz. The higher the frequency, the less jitter there is.
 *	FBDIV must be in the range 16..320. This is actually the multiplication factor.
 *
 * So if we choose FBDIV = 133, we have a VCO output frequeny of 1596, which is in range (just!)
 * Then we need to divide by 12 by setting POSTDIV1 to 6 and POSTDIV2 to 2
*/
#define DV_PLL_SYS_BASE		0x40028000
#define DV_PLL_USB_BASE		0x4002c000

typedef struct dv_rp2040_pll_s
{
	volatile dv_u32_t	cs;			/* 0x00 - control and status */
	volatile dv_u32_t	pwr;		/* 0x04 - controls power modes */
	volatile dv_u32_t	fbdiv_int;	/* 0x08 - feedback divisor */
	volatile dv_u32_t	prim;		/* 0x0c - controls PLL post dividers */
} dv_rp2040_pll_t;

#define dv_rp2040_pll			(((dv_rp2040_pll_t *)(DV_PLL_SYS_BASE))[0])
#define dv_rp2040_pll_xor		(((dv_rp2040_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_pll_w1s		(((dv_rp2040_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_pll_w1c		(((dv_rp2040_pll_t *)(DV_PLL_SYS_BASE+DV_OFFSET_W1C))[0])

#define dv_rp2040_usbpll		(((dv_rp2040_pll_t *)(DV_PLL_USB_BASE))[0])
#define dv_rp2040_usbpll_xor	(((dv_rp2040_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_usbpll_w1s	(((dv_rp2040_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_usbpll_w1c	(((dv_rp2040_pll_t *)(DV_PLL_USB_BASE+DV_OFFSET_W1C))[0])

#define DV_PLL_LOCK			0x80000000	/* CS */
#define DV_PLL_BYPASS		0x00000100	/* CS */
#define DV_PLL_REFDIV		0x0000003f	/* CS */
#define DV_PLL_VCOPD		0x00000020	/* PWR */
#define DV_PLL_POSTDIVPD	0x00000008	/* PWR */
#define DV_PLL_DSMPD		0x00000004	/* PWR */
#define DV_PLL_PD			0x00000001	/* PWR */
#define DV_PLL_FBDIV		0x00000fff	/* FBDIV_INT */
#define DV_PLL_POSTDIV1		0x00070000	/* PRIM */
#define DV_PLL_POSTDIV2		0x00007000	/* PRIM */

extern void dv_clock_init(void);
extern void dv_pll_init(void);

#endif
