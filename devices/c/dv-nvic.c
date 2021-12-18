/*  dv-nvic.c
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
#include "dv-nvic.h"

/* Global variable that holds the "virtual" lock level.
 * 0 ==> All open
 * MAX ==> All locked
*/
dv_intlevel_t dv_currentlocklevel;

#if DV_ARMv6_M

/* Armv6-m doesn't provide a baseprio register so there's no way to selectively block interrupts below
 * a certain priority.
 *
 * To fake that feature we selectively mask and unmask the registers using these variables to
 * keep track of what we're doing
*/
dv_u32_t dv_nvic_enabled;		/* Which interrupts are enabled */
dv_u32_t dv_nvic_levelmask[4];	/* Which interrupts are enabled at each level. 0 is always "all" */

#endif

/* dv_nvic_init() - initialize the nvic
*/
void dv_nvic_init(void)
{
	/* Disable all sources and clear any pending
	*/
	int nsets = (DV_NVECTOR+31)/32;

	for ( int i = 0; i < nsets; i++ )
	{
		dv_nvic.icer[i] = 0xffffffff;	/* Disable all interrupts sources in the set */
		dv_nvic.icpr[i] = 0xffffffff;	/* Clear all pending IRQa in the set */
	}

#if DV_ARMv6_M
	/* Initialise the fake interrupt level variables */
	dv_nvic_enabled = 0;
	dv_nvic_levelmask[0] = 0xffffffff;
	dv_nvic_levelmask[1] = 0;
	dv_nvic_levelmask[2] = 0;
	dv_nvic_levelmask[3] = 0;
#endif
}
