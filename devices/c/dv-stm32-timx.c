/*  dv-stm32-timx.c
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
#include "dv-stm32-timx.h"
#include "dv-stm32-rcc.h"

const dv_timxdescr_t dv_timxdescr[DV_NTIM] = { DV_TIMX_LIST };

/* dv_stm32_timx_init() - initialise a timer
 *
 * Parameter:
 *	- t is timer number, 1..14, validity depends on device.
 *	- p is the prescaler, range 1..65536
*/
int dv_stm32_timx_init(int t, unsigned p)
{
	/* Sanity check
	*/
	if ( t < 1 || t > DV_NTIM )
		return -1;

	/* Enable the clock to the timer
	*/
	*(dv_timxdescr[t-1].apben) |= dv_timxdescr[t-1].mask;

	dv_timx_t *timx = dv_get_timx(t);

	/* Enable the counter
	*/
	timx->cr[0] = DV_TIM_CEN;

	/* Set reload value to full range - this should make the counter free-running
	*/
	timx->arr = 0xffff;

	/* Set the prescaler as specified
	*/
	timx->psc = p-1;

	/* Reset the counter
	*/
	timx->egr = DV_TIM_UG;

	return 0;
}
