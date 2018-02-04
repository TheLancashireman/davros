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

#if DV_N_CORES > 0

#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-mempage.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-c0.h>
#include <kernel/h/dv-coreconfig.h>

/* Memory pages. Here temporarily until memory protection.
*/
dv_page_t dv_c0_pages[DV_C0_N_PAGES] __attribute__((section("dv_pages")));

/* The kernel stack.
 * The stack is here temporarily until memory protection. We should use one of the pages eventually.
*/
dv_stackword_t dv_c0_kernelstack[DV_KSTACK_WORDS+DV_STACKEXTRA] __attribute__((section("dv_stack")));

/* Kernel variables.
*/
dv_kernel_t dv_c0_kvars;

/* Top of kernel stack. Used by assembly-language startup code.
*/
dv_stackword_t *const dv_c0_kernstacktop = &dv_c0_kernelstack[DV_KSTACK_WORDS];

/* Executable table.
 * For multi-core operation this will have to change.
 * Executables will need to be uniquely identifiable across all cores.
 * So either a global mapping table or a global "const" table and core-local state.
*/
dv_executable_t dv_c0_executables[DV_C0_N_EXECUTABLES];

/* Thread table.
*/
dv_thread_t dv_c0_threads[DV_C0_N_THREADS];

/* Registers table.
*/
dv_registers_t dv_c0_registers[DV_C0_N_REGISTERS];

/* Event status table.
*/
dv_eventstatus_t dv_c0_eventstatus[DV_C0_N_EVENTSTATUS];

/* DLL element table for blocking executables.
*/
dv_dllelement_t dv_c0_dllelement[DV_C0_N_DLLELEMENT];

/* Ring buffers for thread job queues, message queues etc.
*/
dv_ringbuffer_t dv_c0_ringbuffers[DV_C0_N_RINGBUFFERS];
dv_u32_t dv_c0_rb_buffers[DV_C0_N_RINGBUFFERWORDS];

/* Page management array.
*/
dv_mempage_t dv_c0_mempage[DV_C0_N_PAGES];


/* Configuration for idle executable.
*/
const dv_execonfig_t dv_c0_cfg_idle =
{	"c0_idle",
	DV_IDLE_FUNC,
	0,
	DV_IDLE_STACK,
	DV_MIN_PRIORITY,
	1,
	0
};

/* Configuration for init/main executable.
*/
const dv_execonfig_t dv_c0_cfg_init =
{	"c0_init",
	DV_C0_INIT_FUNC,
	0,
	DV_C0_INIT_STACK,
	DV_MAX_PRIORITY,
	1,
	DV_EXEFLAG_AUTODESTROY
};

const dv_coreconfig_t dv_c0_coreconfig =
{
	.kernelvars			= &dv_c0_kvars,
	.kernelstack		= &dv_c0_kernelstack[0],
	.kernelstacktop		= &dv_c0_kernelstack[DV_KSTACK_WORDS],
	.executables		= &dv_c0_executables[0],
	.threads			= &dv_c0_threads[0],
	.registers			= &dv_c0_registers[0],
	.events				= &dv_c0_eventstatus[0],
	.dll_elements		= &dv_c0_dllelement[0],
	.ringbuffers		= &dv_c0_ringbuffers[0],
	.buffers			= &dv_c0_rb_buffers[0],
	.pages				= &dv_c0_pages[0],
	.mempage			= &dv_c0_mempage[0],
	.idle_cfg			= &dv_c0_cfg_idle,
	.init_cfg			= &dv_c0_cfg_init,

	.core_index			= 0,
	.n_kernelstack		= DV_KSTACK_WORDS+DV_STACKEXTRA,
	.n_executables		= DV_C0_N_EXECUTABLES,
	.n_threads			= DV_C0_N_THREADS,
	.n_registers		= DV_C0_N_REGISTERS,
	.n_events			= DV_C0_N_EVENTSTATUS,
	.n_dll_elements		= DV_C0_N_DLLELEMENT,
	.n_ringbuffers		= DV_C0_N_RINGBUFFERS,
	.n_ringbufferwords	= DV_C0_N_RINGBUFFERWORDS,
	.n_pages			= DV_C0_N_PAGES
};

#else
const char *dv_c0_dummy = "dv-c0.c";
#endif
