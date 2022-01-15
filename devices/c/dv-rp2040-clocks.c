/*  dv-rp2040-clocks.c
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
#include "dv-rp2040-clocks.h"
#include "dv-rp2040-resets.h"

void dv_clock_init(void)
{
	dv_rp2040_clocks.sys_resus_ctrl = 0x00;				/* Disable resuscitation for now */

	dv_rp2040_xosc.ctrl = DV_XOSC_1_15_MHZ;				/* Should be fixed according to datasheet, but is R/W */
	dv_rp2040_xosc.startup = ((12000000/1000)+255)/256;	/* 1 ms at 12 MHz, rounded up */
	dv_rp2040_xosc_w1s.ctrl = DV_XOSC_ENABLE;
	do {	/* Wait */	} while ( (dv_rp2040_xosc.status & DV_XOSC_STABLE) == 0 );

	dv_rp2040_clocks.ref.ctrl = DV_CLKSRC_REF_XOSC;		/* Select XOSC as the reference clock source */
	dv_rp2040_clocks.sys.ctrl = DV_CLKSRC_SYS_REF;		/* Select REF as the system clock source */

	dv_rp2040_clocks.peri.ctrl = DV_CLK_ENABLE | DV_CLKSRC_PERI_XOSC;		/* Select xosc as peripheral clock */
}

#define MY_REFDIV		1		/* VCO input at 12 MHz */
#define MY_FBDIV		133		/* VCO at 1596 MHz */
#define MY_POSTDIV1		6		/* Div1 output at 266 MHz */
#define MY_POSTDIV2		2		/* Div2 output at 133 MHz */
#define MY_POSTDIVS		((MY_POSTDIV1 << 16) | (MY_POSTDIV2 << 12))

static void dv_pll_helper(dv_rp2040_pll_t *pll, dv_rp2040_pll_t *pll_w1c, dv_u32_t fbdiv, dv_u32_t postdivs);

void dv_pll_init(void)
{
	/* Check if PLL is already correctly configured and running
	*/
	if ( (dv_rp2040_pll.cs & DV_PLL_LOCK) != 0 &&
		 (dv_rp2040_pll.cs & DV_PLL_REFDIV) == MY_REFDIV &&
		 (dv_rp2040_pll.fbdiv_int & DV_PLL_FBDIV) == MY_FBDIV &&
		 (dv_rp2040_pll.prim & (DV_PLL_POSTDIV1 | DV_PLL_POSTDIV2)) == MY_POSTDIVS )
		return;

	/* Reset and re-enable the PLL.
	*/
	dv_rp2040_reset(DV_RESETS_pll_sys);

	/* Initialise the main PLL
	*/
	dv_pll_helper(&dv_rp2040_pll, &dv_rp2040_pll_w1c, MY_FBDIV, MY_POSTDIVS);

	/* Switch the sys clock to the PLL
	*/
	dv_rp2040_clocks.sys.ctrl = DV_CLKSRC_SYS_REF | DV_CLKSRC_SYS_AUX_PLL;	/* Should be this already */
	dv_rp2040_clocks.sys.ctrl = DV_CLKSRC_SYS_AUX | DV_CLKSRC_SYS_AUX_PLL;	/* Switch to the aux clock */
}

/* MY_REFDIV		1  same as main PLL */
#define MY_USB_FBDIV		120		/* VCO at 1440 MHz */
#define MY_USB_POSTDIV1		6		/* Div1 output at 240 MHz */
#define MY_USB_POSTDIV2		5		/* Div2 output at 48 MHz */
#define MY_USB_POSTDIVS		((MY_USB_POSTDIV1 << 16) | (MY_USB_POSTDIV2 << 12))

void dv_usbpll_init(void)
{
	/* Check if USB PLL is already correctly configured and running
	*/
	if ( (dv_rp2040_usbpll.cs & DV_PLL_LOCK) != 0 &&
		 (dv_rp2040_usbpll.cs & DV_PLL_REFDIV) == MY_REFDIV &&
		 (dv_rp2040_usbpll.fbdiv_int & DV_PLL_FBDIV) == MY_USB_FBDIV &&
		 (dv_rp2040_usbpll.prim & (DV_PLL_POSTDIV1 | DV_PLL_POSTDIV2)) == MY_USB_POSTDIVS )
		return;

	/* Reset and re-enable the PLL.
	*/
	dv_rp2040_reset(DV_RESETS_pll_usb);

	/* Initialise the USB PLL
	*/
	dv_pll_helper(&dv_rp2040_usbpll, &dv_rp2040_usbpll_w1c, MY_USB_FBDIV, MY_USB_POSTDIVS);

	/* Switch the USB clock to the PLL
	*/
	dv_rp2040_clocks.usb.div = DV_CLK_DIVBY1;
	dv_rp2040_clocks.usb.ctrl = DV_CLK_ENABLE | DV_CLKSRC_USB_PLL_USB;
}

static void dv_pll_helper(dv_rp2040_pll_t *pll, dv_rp2040_pll_t *pll_w1c, dv_u32_t fbdiv, dv_u32_t postdivs)
{
	/* Load the VCO-related dividers
	*/
	pll->cs = MY_REFDIV;
	pll->fbdiv_int = fbdiv;

	/* Power on the PLL
	*/
	pll_w1c->pwr = DV_PLL_VCOPD | DV_PLL_PD;

	/* Wait for the PLL to lock
	*/
	do {	/* Wait */	} while ( (pll->cs & DV_PLL_LOCK) == 0 );

	/* Configure the post-dividers
	*/
	pll->prim = postdivs;

	/* Power on the post-dividers
	*/
	pll_w1c->pwr = DV_PLL_POSTDIVPD;
}
