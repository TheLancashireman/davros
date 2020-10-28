/*	dv-allocatethread.c - allocate a thread for an executable
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
	thr_tbl[thr_i].jobqueue = DV_NULL;

	return &thr_tbl[thr_i];
}

/* dv_deallocate_thread() - deallocate a thread
 *
 * There's a small problem here; what to do about the job queue?
 *
 * Deallocating the ringbuffer structure is simple. However, the job queue has allocated some memory for the
 * buffer storage, and we would have to release that to the pool. Eventually, the pool would become
 * fragmented.
 * One possibility might be to compact the pool to remove the gap.
 * For now, however, we'll leave the job queue attached to the thread.
*/
void dv_deallocate_thread(dv_kernel_t *kvars, dv_thread_t *thr)
{
	dv_assert((thr->n_exe > 0), dv_panic_initialisationerror, "dv_deallocate_thread", "in-use counter error");

	thr->n_exe--;

	if ( thr->n_exe <= 0 )
	{
#if 0
		if ( thr->jobqueue != DV_NULL )
		{
			dv_deallocate_ringbuffer(kvars, thr->jobqueue);
			thr->jobqueue = DV_NULL;
		}
#endif

		dv_deallocate_obj(&kvars->thr_allocator);
	}
}
