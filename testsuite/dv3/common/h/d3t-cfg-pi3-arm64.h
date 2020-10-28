/*	d3t-cfg-pi3-arm64.h - davros configuration for raspberry pi 3 (aarch64)
 *
 *	Copyright David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef d3t_cfg_pi3_arm64_h
#define d3t_cfg_pi3_arm64_h	1

#define DV_BOARDCONFIG		<board/arm64/raspberry-pi-3/h/dv-boardconfig.h>
#define DV_TARGET_CORE		0

#define DV_PRJ_VBAR_EL3		((dv_u64_t)&prj_vectors)
#define DV_PRJ_VBAR_EL2		((dv_u64_t)&prj_vectors)

#if !DV_ASM
extern void *d3t_vectors;
#endif

#endif
