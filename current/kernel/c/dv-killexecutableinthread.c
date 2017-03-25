/*	dv-killexecutableinthread.c - kill the executable that is running in a thread.
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
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-trace.h>
#include <kernel/h/dv-coverage.h>

DV_COVDEF(kill_executable_in_thread);

/* dv_kill_executable_in_thread() - kill the executable in the specified thread.
 *
 * Kill the specified executable, which is running in the specified thread.
*/
void dv_kill_executable_in_thread(dv_kernel_t *kvars, dv_thread_t *thr, dv_executable_t *exe)
{
	exe->n_instances--;

	dv_dllremove(&thr->link);

	dv_trace_threadstate(thr, dv_thread_idle);
	thr->state = dv_thread_idle;
	
	thr->executable = DV_NULL;
	thr->regs = DV_NULL;
	thr->parent = DV_NULL;

	while ( thr->locktaken != DV_NULL )
	{
		dv_panic(dv_panic_unimplemented, "dv_kill_executable_in_thread", "Locks not implemented yet");
	}

	kvars->current_thread = DV_NULL;

	/* Job queue handling might move up a level ...
	*/
	if ( thr->jobqueue != DV_NULL )
	{
		dv_panic(dv_panic_unimplemented, "dv_kill_executable_in_thread", "Job queues not implemented yet");
	}
}

/* man-page-generation - to be defined
*/
