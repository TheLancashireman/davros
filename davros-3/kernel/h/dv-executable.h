/*	dv-executable.h - executable object for davros
 *
 *	Copyright 2015 David Haworth
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
#ifndef dv_executable_h
#define dv_executable_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

#if !DV_ASM

/* dv_execonfig_s
 *
 * A structure to describe an executable for configuration.
*/
struct dv_execonfig_s
{
	const char *name;				/* The name of the executable */
	dv_function_t function;			/* The top-level function */
	dv_index_t core;				/* The core on which this executable runs */
	dv_quantity_t n_stack;			/* Stack requirement (no. of stackwords) */
	dv_i32_t priority;				/* Configured priority */
	dv_quantity_t maxinstances;		/* No. of instances (for "basic tasks") */
	dv_u32_t flags;					/* Configuration options (see below) */
};

/* dv_exestate_t - State of an executable
*/
typedef enum dv_exestate_e
{
	dv_exe_disabled = 0,	/* Not permitted to run - might need allocations (stack, events etc.)  */
	dv_exe_idle,			/* Idle. Must spawn to start it running */
    dv_exe_active,			/* Occupying thread and ready to run when scheduled */
    dv_exe_suspended,		/* Spawned but doing nothing */
	dv_exe_sleep,			/* Sleeping */
	dv_exe_lock_wait,		/* Waiting for a lock */
	dv_exe_event_wait,		/* Waiting for an event */
    dv_exe_nstates
} dv_exestate_t;

/* dv_executable_s
 *
 * A representation of an executable object in davros.
 * An exectuable object is a function plus the attributes needed to manage its execution.
 * An executable is NOT an executing instance of the function - that's managed using a thread.
*/
struct dv_executable_s
{
	const char *name;
	dv_function_t function;
	dv_thread_t *thread;
	dv_registers_t *registers;
	dv_dllelement_t *dll_element;	/* For sleep, locks etc. */
	dv_eventstatus_t *events;
	dv_mempage_t *stackpage;
	dv_stackword_t *initial_sp;
	dv_quantity_t n_stack;
	dv_index_t id;
	dv_i32_t baseprio;
	dv_i32_t runprio;
	dv_i32_t maxprio;
	dv_quantity_t maxinstances;		/* For OSEK-style multiple activations */
	dv_u32_t flags;

	dv_quantity_t n_instances;
	dv_exestate_t state;
};

#define DV_EXEFLAG_SYNCHRONOUS		0x00000001		/* Can be activated synchronously */
#define DV_EXEFLAG_ASYNCHRONOUS		0x00000002		/* Can be activated asynchronously */
#define DV_EXEFLAG_CALL				0x00000004		/* Activations have "call" semantics */
#define DV_EXEFLAG_BLOCKING			0x00000008		/* Can use "blocking" APIs */
#define DV_EXEFLAG_EVENTS			0x00000010		/* Can use events */
#define DV_EXEFLAG_AUTODESTROY		0x00000020		/* Automatically destroy on termination */

dv_index_t dv_create_executable(dv_kernel_t *, const dv_execonfig_t *);
dv_errorid_t dv_spawn_executable(dv_kernel_t *, dv_executable_t *);
void dv_spawn_executable_in_thread(dv_doublylinkedlist_t *, dv_executable_t *, dv_thread_t *);
void dv_resume_executable_in_thread(dv_doublylinkedlist_t *, dv_executable_t *, dv_thread_t *);
void dv_kill_executable_in_thread(dv_kernel_t *, dv_thread_t *);
void dv_remove_executable_from_thread(dv_kernel_t *, dv_thread_t *);
dv_u64_t dv_wakeup(dv_kernel_t *);

#endif

#endif
