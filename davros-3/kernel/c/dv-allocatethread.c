/*	dv-allocatethread.c - allocate a thread for an executable
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
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-ringbuffer.h>

DV_COVDEF(allocatethread);

/* dv_is_free_thread() - return true if thread i is free
*/
static dv_boolean_t dv_is_free_thread(dv_index_t i, const void *tbl)
{
	const dv_thread_t *thr_tbl = tbl;
	if ( thr_tbl[i].n_exe == 0 )
		return 1;
	return 0;
}

/* dv_allocate_thread() - allocate a thread
 *
 * Find and allocate a thread for an executable.
 * Searches the executable table for another executable with the same priority and shares
 * the thread.
 * There will be some exceptions to the sharing rule depending on the executable type.
*/
dv_thread_t *dv_allocate_thread(dv_kernel_t *kvars, const dv_executable_t *exe)
{
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_thread_t *thr_tbl;
	int i;
	int thr_i;
	int n_thr;

	/* First search all the executables to find a thread to share.
	*/
	for ( i=0; i<dv_coreconfigs[kvars->core_index]->n_executables; i++ )
	{
		if ( (exe_tbl[i].name != DV_NULL) &&
			 (exe_tbl[i].baseprio == exe->baseprio) &&
			 (exe_tbl[i].thread != DV_NULL) )
		{
			DV_DBG(dv_kprintf("dv_allocate_thread(): %s shares thread 0x%08x with %s\n",
															exe->name, (unsigned)exe_tbl[i].thread, exe_tbl[i].name));
			exe_tbl[i].thread->n_exe++;

			if ( exe_tbl[i].thread->jobqueue == DV_NULL )
			{
				/* If there's no job queue we allocate one.
				*/
				exe_tbl[i].thread->jobqueue = dv_allocate_ringbuffer(kvars);

				if ( exe_tbl[i].thread->jobqueue == DV_NULL )
					dv_panic(dv_panic_objectsearchfailed, "dv_allocate_thread", "no ringbuffer available");

				/* When we allocate a job queue for a shared thread, the original first executable in
				 * the thread had a single instance. So the job queue's initial length is the number
				 * of instances of the new executable.
				*/
				dv_rb_configure(exe_tbl[i].thread->jobqueue, rb_simple, 4, exe->maxinstances);
				dv_kprintf("dv_allocate_thread(): new jobj queue, length is %d\n",
							exe_tbl[i].thread->jobqueue->length);
			}
			else
			{
				dv_rb_lengthen(exe_tbl[i].thread->jobqueue, exe->maxinstances);
				dv_kprintf("dv_allocate_thread(): lengthen jobj queue, length is %d\n",
							exe_tbl[i].thread->jobqueue->length);
			}

			return exe_tbl[i].thread;
		}
	}

	thr_tbl = dv_coreconfigs[kvars->core_index]->threads;
	n_thr = dv_coreconfigs[kvars->core_index]->n_threads;
	thr_i = dv_allocate_obj(&kvars->thr_allocator, n_thr, dv_is_free_thread, thr_tbl);

	if ( thr_i < 0 )
		return DV_NULL;

	thr_tbl[thr_i].n_exe = 1;
	thr_tbl[thr_i].link.payload_type = dv_dll_thread;
	thr_tbl[thr_i].link.payload = &thr_tbl[thr_i];
	if ( exe-> maxinstances > 1 )
	{
		/* Multiple instances: we need a job queue immediately.
		*/
		thr_tbl[thr_i].jobqueue = dv_allocate_ringbuffer(kvars);

		if ( thr_tbl[thr_i].jobqueue == DV_NULL )
		{
			/* ToDo: better error handling */
			dv_panic(dv_panic_objectsearchfailed, "dv_allocate_thread", "no ringbuffer available");
		}
		else
		{
			/* When the first executable has multiple activations the job queue
			 * needs one fewer entries because the first activation doesn't go in the queue.
			*/
			dv_rb_configure(thr_tbl[thr_i].jobqueue, rb_simple, 4, exe->maxinstances-1);
		}
	}
	else
	{
		thr_tbl[thr_i].jobqueue = DV_NULL;
	}

	return &thr_tbl[thr_i];
}
