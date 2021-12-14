/* dv-target-pi-pico.h - header file for davroska on a Raspberry Pi pico
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
#ifndef dv_target_pi_pico_h
#define dv_target_pi_pico_h	1

#include <arm/h/dv-types-cortexm.h>
#include <dv-cortex-m.h>
#include <dv-stdio.h>

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-nvic.h>

#define DV_MAX_INTLEVEL		15
#define DV_LOCKALL_LEVEL	0
#define DV_NVECTOR			32
#define DV_ARMv6_M			1

#endif
