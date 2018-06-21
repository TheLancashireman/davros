/*	dv-projectconfig.h - davros configuration for loadhigh project
 *
 *	Copyright 2018 David Haworth
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

#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>

/* loadhigh doesn't use davros as an OS, so this is a dummy configuration
*/
#define DV_MEMSIZE_MB			16

#define DV_C0_N_EXECUTABLES		2
#define DV_C0_N_THREADS			2
#define DV_C0_N_REGISTERS		2
#define DV_C0_N_EVENTSTATUS		1
#define DV_C0_N_DLLELEMENT		1
#define DV_C0_N_RINGBUFFERS		1
#define DV_C0_N_RINGBUFFERWORDS	1

#define DV_C0_N_PAGES			1

#define DV_C0_INIT_FUNC			prj_init
#define DV_C0_INIT_STACK		32

#define DV_TRACE				0
#define DV_DEBUG				0

#define DV_PRJ_STARTUP			0

#if !DV_ASM
void prj_init(void);
#endif

#endif
