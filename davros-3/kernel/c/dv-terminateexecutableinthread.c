/*	dv-terminateexecutableinthread.c - terminate the executable that is running in a thread.
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
#include <dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-stdio.h>

DV_COVDEF(terminate_executable_in_thread);

/* dv_terminate_executable_in_thread() - terminate the executable in the specified thread.
 *
 * Terminate the executable that is running in the specified thread.
*/
void dv_terminate_executable_in_thread(dv_kernel_t *kvars, dv_thread_t *thr)
{
	thr->executable->state = dv_exe_idle;

	dv_remove_executable_from_thread(kvars, thr);
}

/* dv_remove_executable_from_thread() - remove the executable from the specified thread.
 *
 * Remove an executable from its thread when terminated or blocked.
*/
void dv_remove_executable_from_thread(dv_kernel_t *kvars, dv_thread_t *thr)
{
	dv_dllremove(&thr->link);

	dv_trace_threadstate(thr, dv_thread_idle);
	thr->state = dv_thread_idle;

	thr->executable = DV_NULL;
	thr->regs = DV_NULL;
	thr->parent = DV_NULL;

	while ( thr->semtaken != DV_NULL )
	{
		dv_panic(dv_panic_unimplemented, "dv_terminate_executable_in_thread", "Semaphores not implemented yet");
	}

	kvars->current_thread = DV_NULL;

	/* Job queue handling might move up a level ...
	*/
	if ( thr->jobqueue != DV_NULL )
	{
		dv_index_t exe_id;

		if ( dv_rb_remove_simple(thr->jobqueue, &exe_id) == 0 )
		{
			/* Nothing to do. */
			DV_DBG(dv_kprintf("dv_remove_executable_from_thread() - job queue empty\n"));
		}
		else
		if ( exe_id >= 0 && exe_id < dv_coreconfigs[kvars->core_index]->n_executables )
		{
			dv_executable_t *exe = &dv_coreconfigs[kvars->core_index]->executables[exe_id];

			DV_DBG(dv_kprintf("dv_remove_executable_from_thread() - job in queue: %s\n", exe->name));
			dv_spawn_executable_in_thread(&kvars->thread_queue, exe, thr);
		}
		else
		{
			dv_panic(dv_panic_internalerror, "dv_remove_executable_from_thread", "unknown executable in job queue");
		}
	}
}
