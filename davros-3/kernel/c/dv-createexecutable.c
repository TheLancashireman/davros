/*	dv-createexecutable.c - create a new executable object
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-mempage.h>

DV_COVDEF(create_executable);

/* dv_is_free_exe() - retrun true if executable i is free
*/
static dv_boolean_t dv_is_free_exe(dv_index_t i, const void *tbl)
{
	const dv_executable_t *exe_tbl = tbl;
	if ( exe_tbl[i].name == DV_NULL )
		return 1;
	return 0;
}


/* dv_create_executable() - create a new executable
 *
 * Preconditions:
 *	- execfg is valid
 *	- execfg is for this core
 *	- exe_allocator.n_free > 0
*/
dv_index_t dv_create_executable(dv_kernel_t *kvars, const dv_execonfig_t *execfg)
{
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_quantity_t n_exe = dv_coreconfigs[kvars->core_index]->n_executables;
	dv_index_t exe_i = dv_allocate_obj(&kvars->exe_allocator, n_exe, dv_is_free_exe, exe_tbl);
	dv_executable_t *exe = DV_NULL;

	if ( exe_i < 0 )
	{
		dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no free executable found");
	}
	else
	{
		exe = &exe_tbl[exe_i];

		/* Configure the executable.
		*/
		exe->name = execfg->name;
		exe->function = execfg->function;
		exe->id = exe_i;
		exe->baseprio = execfg->priority;
		exe->runprio = execfg->priority;
		exe->maxprio = execfg->priority;
		exe->maxinstances = execfg->maxinstances;
		exe->n_stack = execfg->n_stack;
		exe->flags = execfg->flags;
		exe->state = dv_exe_disabled;
		exe->n_instances = 0;
		exe->thread = DV_NULL;
		exe->registers = DV_NULL;
		exe->events = DV_NULL;
		exe->dll_element = DV_NULL;
		exe->stackpage = DV_NULL;

		exe->thread = dv_allocate_thread(kvars, exe);
		if ( exe->thread == DV_NULL )
		{
			/* ToDo: better error handling */
			dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no thread available");
		}

		exe->registers = dv_allocate_registers(kvars, exe);
		if ( exe->registers == DV_NULL )
		{
			/* ToDo: better error handling */
			dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no registers available");
		}

		if ( exe->flags & DV_EXEFLAG_EVENTS )
		{
			exe->events = dv_allocate_eventstatus(kvars);
			if ( exe->events == DV_NULL )
			{
				/* ToDo: better error handling */
				dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no eventstatus available");
			}
		}

		if ( exe->flags & DV_EXEFLAG_BLOCKING )
		{
			exe->dll_element = dv_allocate_dllelement(kvars);
			if ( exe->dll_element == DV_NULL )
			{
				/* ToDo: better error handling */
				dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no dll_element available");
			}
		}

		exe->stackpage = dv_allocate_stack(kvars, exe);
		if ( exe->stackpage == DV_NULL )
		{
			/* ToDo: better error handling */
			dv_panic(dv_panic_objectsearchfailed, "dv_create_executable", "no stack available");
		}
		exe->initial_sp = &exe->stackpage->page->words[exe->n_stack - DV_STACKEXTRA];

		/* If all OK, enable the new executable.
		*/
		exe->state = dv_exe_idle;
	}

	return exe_i;
}

/* man-page-generation - to be defined
*/
