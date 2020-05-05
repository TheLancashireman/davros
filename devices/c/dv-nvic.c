/*  dv-nvic.c
extern const int dv_nvic_nirq;
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

const int dv_nvic_nirq = DV_NVECTOR;

/* Global variable that holds the "virtual" lock level.
 * 0 ==> All open
 * MAX ==> All locked
*/
dv_intlevel_t dv_currentlocklevel;

/* dv_nvic_init() - initialize the nvic
*/
void dv_nvic_init(void)
{
	/* Disable all sources and clear any pending
	*/
	int nsets = (dv_nvic_nirq+31)/32;

	for ( int i = 0; i < nsets; i++ )
	{
		dv_nvic.icer[i] = 0xffffffff;	/* Disable all interrupts sources in the set */
		dv_nvic.icpr[i] = 0xffffffff;	/* Clear all pending IRQa in the set */
	}
}
