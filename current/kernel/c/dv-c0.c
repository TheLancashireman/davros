/*	dv-c0.c - core 0 data
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
#include <kernel/h/dv-kconfig.h>

#if DV_NCORES > 0

#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include DV_REGISTERS
#include <kernel/h/dv-c0.h>

/* Kernel variables.
*/
dv_kernel_t dv_c0_kvars;

/* The kernel stack.
 * The stack is here temporarily until memory protection.
*/
dv_stackword_t dv_c0_kernelstack[DV_KSTACK_WORDS+DV_STACKEXTRA];

/* Top of kernel stack. Used by assembly-language startup code.
*/
dv_stackword_t *const dv_c0_kernstacktop = &dv_c0_kernelstack[DV_KSTACK_WORDS];

/* Executable table.
 * For multi-core operation this will have to change.
 * Executables will need to be uniquely identifiable across all cores.
 * So either a global mapping table or a global "const" table and core-local state.
*/
dv_executable_t dv_c0_executables[DV_C0_NEXECUTABLES];

/* Thread table.
*/
dv_thread_t dv_c0_threads[DV_C0_NTHREADS];

/* Registers table.
*/
dv_registers_t dv_c0_registers[DV_C0_NREGISTERS];

/* Event status table.
*/
dv_eventstatus_t dv_c0_eventstatus[DV_C0_NEVENTSTATUS];

#else
const char *dv_c0_dummy = "dv-c0.c";
#endif
