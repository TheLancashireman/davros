/*	dv-projectconfig.h - davros configuration for xxx project
 *
 *	Copyright 2020 David Haworth
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

#include "dt3-testconfig.h"

/* Include the board config header.
 * The name of the board configuration header is defined on the compiler command line
*/
#include DT3_H_BOARDCFG

#include <kernel/h/dv-kernel-types.h>

/* Provide default davros-3 configuration items for those not defined by the test case.
*/
#ifndef DV_MEMSIZE_MB
#define DV_MEMSIZE_MB			480
#endif

#ifndef DV_C0_N_EXECUTABLES
#define DV_C0_N_EXECUTABLES		20
#endif

#ifndef DV_C0_N_THREADS
#define DV_C0_N_THREADS			20
#endif

#ifndef DV_C0_N_REGISTERS
#define DV_C0_N_REGISTERS		20
#endif

#ifndef DV_C0_N_EVENTSTATUS
#define DV_C0_N_EVENTSTATUS		5
#endif

#ifndef DV_C0_N_DLLELEMENT
#define DV_C0_N_DLLELEMENT		5
#endif

#ifndef DV_C0_N_RINGBUFFERS
#define DV_C0_N_RINGBUFFERS		10
#endif

#ifndef DV_C0_N_RINGBUFFERWORDS
#define DV_C0_N_RINGBUFFERWORDS	100
#endif

#ifndef DV_C0_N_PAGES
#define DV_C0_N_PAGES			100000
#endif

#ifndef DV_C0_INIT_FUNC
#define DV_C0_INIT_FUNC			prj_init
#endif

#ifndef DV_C0_INIT_STACK
#define DV_C0_INIT_STACK		200
#endif

#ifndef DV_TRACE
#define DV_TRACE				0
#endif

#ifndef DV_DEBUG
#define DV_DEBUG				0
#endif

#ifndef DV_PRJ_STARTUP
#define DV_PRJ_STARTUP			1
#endif

#if !DV_ASM
void prj_init(void);
void prj_startup(dv_kernel_t *kvars);
#endif

#endif
