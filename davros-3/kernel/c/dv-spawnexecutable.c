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
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(spawn_executable);

/* Append executable's ID to job queue.
*/
static inline void dv_enqueue_job_in_jobqueue(dv_kernel_t *kvars, dv_executable_t *exe)
{
	if ( exe->thread->jobqueue == DV_NULL )
		dv_panic(dv_panic_initialisationerror, "dv_spawn_executable", "no jobqueue allocated");

	if ( exe->thread->jobqueue->buf == DV_NULL )
	{
		dv_rb_allocate(kvars, exe->thread->jobqueue);
	}

	if ( dv_rb_append_simple(exe->thread->jobqueue, &exe->id) == 0 )
		dv_panic(dv_panic_initialisationerror, "dv_spawn_executable", "insufficient space in jobqueue");
}

/* dv_spawn_executable() - spawn an instance of an executable
*/
dv_errorid_t dv_spawn_executable(dv_kernel_t *kvars, dv_executable_t *exe)
{
	dv_errorid_t ecode = dv_eid_UnknownError;

	if ( exe->state == dv_exe_disabled )
	{
		ecode = dv_eid_ExecutableQuarantined;
	}
	else
	{
		if ( exe->n_instances < exe->maxinstances )
		{
			exe->n_instances++;

			if ( exe->events != DV_NULL )
			{
				exe->events->pending_events = DV_NO_EVENTS;
				exe->events->awaited_events = DV_NO_EVENTS;
			}

			if ( exe->dll_element != DV_NULL )
			{
				exe->dll_element->successor = DV_NULL;
				exe->dll_element->predecessor = DV_NULL;
				exe->dll_element->key.u64_key = 0;
				exe->dll_element->payload_type = dv_dll_exe;
				exe->dll_element->payload = exe;
			}

			if ( exe->thread->state == dv_thread_idle )
				dv_spawn_executable_in_thread(&kvars->thread_queue, exe, exe->thread);
			else
				dv_enqueue_job_in_jobqueue(kvars, exe);

			ecode = dv_eid_None;
		}
		else
		{
			ecode = dv_eid_MaxInstancesExceeded;
		}
	}

	return ecode;
}
