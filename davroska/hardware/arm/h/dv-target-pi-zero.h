/* dv-target-pi-zero.h - header file for davroska on a pi-zero
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/
#ifndef dv_target_pi_zero_h
#define dv_target_pi_zero_h	1

#include <arm/h/dv-types-arm.h>
#include <dv-stdio.h>

#define DV_PBASE	0x20000000

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-arm-bcm2835-interruptcontroller.h>
#define DV_SUPPORT_INTLEVEL	1
#define DV_MAX_INTLEVEL		7
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL+1)
#define DV_NVECTOR			dv_n_bcm2835_iid

#endif
