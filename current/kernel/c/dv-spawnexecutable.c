/*	dv-spawnexecutable.c - spawn an instance of an executable object
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-error.h>

/* Todo: job queues.
*/
#define dv_enqueue_job_in_jobqueue(x, y) \
	dv_panic(dv_panic_unimplemented, "dv_spawn_executable", "jobqueue not implemented")

DV_COVDEF(spawn_executable);

/* dv_spawn_executable() - spawn an instance of an executable
*/
dv_errorid_t dv_spawn_executable(dv_kernel_t *kvars, dv_executable_t *exe)
{
	dv_errorid_t ecode = dv_eid_UnknownError;

	if ( exe->enabled )
	{
		if ( exe->n_instances < exe->maxinstances )
		{
			exe->n_instances++;

			if ( exe->events != DV_NULL )
			{
				exe->events->pending_events = DV_NO_EVENTS;
				exe->events->awaited_events = DV_NO_EVENTS;
			}

			if ( exe->thread->state == dv_thread_idle )
				dv_spawn_executable_in_thread(&kvars->thread_queue, exe, exe->thread);
			else
				dv_enqueue_job_in_jobqueue(exe->thread, exe);
		}
		else
		{
			ecode = dv_eid_MaxInstancesExceeded;
		}
	}
	else
	{
		ecode = dv_eid_ExecutableQuarantined;
	}

	return ecode;
}

/* man-page-generation - to be defined
*/
