/* dv-target-pi3-arm64.h - header file for davroska on a pi3 in 64-bit mode
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
#ifndef dv_target_pi3_arm64_h
#define dv_target_pi3_arm64_h	1

#include <arm64/h/dv-types-arm64.h>
#include <dv-stdio.h>

#define DV_PBASE	0x3f000000
#define DV_PBASE2	0x40000000

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-arm-bcm2836-interruptcontroller.h>
#define DV_SUPPORT_INTLEVEL	1
#define DV_MAX_INTLEVEL		7
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL+1)
#define DV_NVECTOR			(dv_n_bcm2836_iid)

#endif
