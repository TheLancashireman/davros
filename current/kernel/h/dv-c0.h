/*	dv-c0.h - davros declarations for core 0
 *
 *	Copyright 2017 David Haworth
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
#ifndef dv_c0_h
#define dv_c0_h	1

#if DV_NCORES > 0

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include DV_REGISTERS

/* See dv-c0.c for a descriptions of the data.
*/

extern dv_kernel_t dv_c0_kvars;

extern dv_stackword_t dv_c0_kernelstack[DV_KSTACK_WORDS+DV_STACKEXTRA];

extern dv_stackword_t *const dv_c0_kernstacktop;

extern dv_executable_t dv_c0_executables[DV_C0_NEXECUTABLES];

extern dv_thread_t dv_c0_threads[DV_C0_NTHREADS];

extern dv_registers_t dv_c0_registers[DV_C0_NREGISTERS];

extern dv_eventstatus_t dv_c0_eventstatus[DV_C0_NEVENTSTATUS];

#endif

#endif
