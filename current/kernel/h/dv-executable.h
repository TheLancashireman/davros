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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

#if !DV_ASM

/* dv_executable_s
 *
 * A representation of an executable object in davros.
 * An exectuable object is a function plus the attributes needed to manage its execution.
 * An executable is NOT an executing instance of the function - that's managed using a thread.
*/
struct dv_executable_s
{
	const dv_char_t *name;
	dv_function_t function;
	dv_thread_t *thread;
	dv_registers_t *registers;
	dv_eventstatus_t *events;
	dv_stackelement_t *stack;
	dv_quantity_t nstack;
	dv_identity_t id;
	dv_dllkey_t baseprio;
	dv_dllkey_t runprio;
	dv_dllkey_t maxprio;
	dv_quantity_t maxinstances;
	dv_uint32_t flags;

	dv_boolean_t enabled;
	dv_quantity_t n_instances;
};

#define DV_EXEFLAG_SYNCHRONOUS		0x00000001		/* Can be activated synchronously */
#define DV_EXEFLAG_ASYNCHRONOUS		0x00000002		/* Can be activated asynchronously */ 
#define DV_EXEFLAG_CALL				0x00000004		/* Activations have "call" semantics */ 
#define DV_EXEFLAG_BLOCKING			0x00000008		/* Can use "blocking" APIs */

dv_errorid_t dv_spawnexecutable(dv_kernel_t *kvars, dv_executable_t *executable);
dv_errorid_t dv_spawnexecutableinthread(dv_doublylinkedlist_t *list, dv_thread_t *thread, dv_executable_t *executable);

#endif



#endif
