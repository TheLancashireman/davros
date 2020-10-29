/*	dv-spawnexecutable.c - spawn an instance of an executable object
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
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(spawn_executable);

/* Ensure that the thread has a job queue
*/
static inline dv_ringbuffer_t *dv_get_thread_jobqueue(dv_kernel_t *kvars, dv_executable_t *exe)
{
	if ( exe->thread->jobqueue == DV_NULL )
	{
		/* If there's no job queue we allocate one.
		*/
		exe->thread->jobqueue = dv_allocate_ringbuffer(kvars);

		if ( exe->thread->jobqueue == DV_NULL )
			dv_panic(dv_panic_objectsearchfailed, "dv_enqueue_job_in_jobqueue", "no ringbuffer available");

		dv_rb_configure(exe->thread->jobqueue, rb_simple, 4, dv_thread_rb_length(exe->thread->n_exe));
		DV_DBG(dv_kprintf("dv_enqueue_job_in_jobqueue(): new jobj queue, length is %d\n",
																	exe->thread->jobqueue->length));

		dv_rb_allocate(kvars, exe->thread->jobqueue);
	}

	return exe->thread->jobqueue;
}

/* Append executable's ID to job queue.
*/
dv_errorid_t dv_enqueue_job_in_jobqueue(dv_kernel_t *kvars, dv_executable_t *exe)
{
	dv_ringbuffer_t *jq = dv_get_thread_jobqueue(kvars, exe);

	if ( dv_rb_append_simple(jq, &exe->id) == 0 )
		return dv_eid_ThreadJobQueueFull;

	return dv_eid_None;
}

/* dv_spawn_executable() - spawn an instance of an executable
*/
dv_errorid_t dv_spawn_executable(dv_kernel_t *kvars, dv_executable_t *exe)
{
	dv_errorid_t ecode = dv_eid_UnknownError;

	exe->state = dv_exe_active;

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
	{
		dv_spawn_executable_in_thread(&kvars->thread_queue, exe, exe->thread);
		ecode = dv_eid_None;
	}
	else
		ecode = dv_enqueue_job_in_jobqueue(kvars, exe);

	return ecode;
}
