/*	dv-destroyexecutable.c - destroy an executable object
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-mempage.h>

DV_COVDEF(destroy_executable);

/* dv_destroy_executable() - destroy an executable
 *
 * Preconditions:
 *	- executable is idle (has been removed from thread)
*/
void dv_destroy_executable(dv_kernel_t *kvars, dv_executable_t *exe)
{
	if ( exe->stackpage != DV_NULL )
	{
		dv_deallocate_stack(kvars, exe->stackpage);
		exe->stackpage = DV_NULL;
	}

	if ( exe->dll_element != DV_NULL )
	{
		dv_deallocate_dllelement(kvars, exe->dll_element);
		exe->dll_element = DV_NULL;
	}

	if ( exe->events != DV_NULL )
	{
		dv_deallocate_eventstatus(kvars, exe->events);
		exe->events = DV_NULL;
	}

	if ( exe->registers != DV_NULL )
	{
		dv_deallocate_registers(kvars, exe->registers);
		exe->registers = DV_NULL;
	}

	if ( exe->thread != DV_NULL )
	{
		dv_deallocate_thread(kvars, exe->thread);
		exe->thread = DV_NULL;
	}

	exe->name = DV_NULL;
	exe->function = DV_NULL;
	exe->id = -1;
	exe->baseprio = 0;
	exe->runprio = 0;
	exe->maxprio = 0;
	exe->n_stack = 0;
	exe->flags = 0;
	exe->state = dv_exe_free;
	dv_deallocate_obj(&kvars->exe_allocator);
}
