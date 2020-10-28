/*	dv-projectconfig.h - davros configuration for xxx project
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
#ifndef dv_projectconfig_h
#define dv_projectconfig_h	1

#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>

#define DV_MEMSIZE_MB			480

#define DV_C0_N_EXECUTABLES		20
#define DV_C0_N_THREADS			20
#define DV_C0_N_REGISTERS		20
#define DV_C0_N_EVENTSTATUS		5
#define DV_C0_N_DLLELEMENT		5
#define DV_C0_N_RINGBUFFERS		10
#define DV_C0_N_RINGBUFFERWORDS	100

#define DV_C0_N_PAGES			100000

#define DV_C0_INIT_FUNC			prj_init
#define DV_C0_INIT_STACK		200

#define DV_TRACE				0
#define DV_DEBUG				1

#define DV_PRJ_STARTUP			0

#define DV_PRJ_VBAR_EL2			((dv_u64_t)&prj_vectors)
#define DV_PRJ_VBAR_EL3			((dv_u64_t)&prj_vectors)

#if !DV_ASM
void prj_init(void);
void prj_startup(dv_kernel_t *kvars);
extern dv_u64_t prj_vectors;
#endif

#endif
