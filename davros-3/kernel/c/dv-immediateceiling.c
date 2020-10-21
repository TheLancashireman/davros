/*	dv-immediateceiling.c - mutexes with immediate ceiling priority protocol
 *
 *	Copyright 2020 David Haworth
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
#include <kernel/h/dv-semaphore.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-error.h>

/* dv_wait_semimmceil() - acquire a mutex with immediate priority ceiling protocol
 *
 * Executables with higher base priority are not allowed to acquire; queueing isn't allowed.
 * The same semaphore can be acquired the sempahore several times.
*/
dv_errorid_t dv_wait_semimmceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	/* It's an error if the caller's base priority is higher than the semaphore's ceiling priority.
	*/
	if ( kvars->current_thread->executable->baseprio > sem->ceiling )
		return dv_eid_SemaphoreCeilingTooLow;

	sem->count--;

	if ( sem->count < 0 )
	{
		dv_assert( (sem->owner != kvars->current_thread),
			dv_panic_semaphoreoccupied, "dv_wait_semimmceil", "Mutex is occupied by a different thread");

		return dv_eid_None;
	}

	/* Acquire the semaphore.
	*/
	dv_acquire_mutex(sem, kvars->current_thread);

	/* Raise the priority of the current thread. Requeueing isn't needed because the caller is at the head.
	*/
	if ( sem->ceiling > sem->oldprio )
		dv_set_prio(kvars->current_thread, sem->ceiling);

	return dv_eid_None;
}

/* dv_signal_semimmceil() - relinquish a mutex with immediate priority ceiling protocol
 *
 * Only the owner is allowed to signal.
*/
dv_errorid_t dv_signal_semimmceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	if ( sem->owner != kvars->current_thread )
		return dv_eid_SemaphoreNotOccupied;

	sem->count++;

	dv_assert( (sem->count < 2),
		dv_panic_semaphorecounterror, "dv_signal_semimmceil", "Unexpected value in semaphore counter");

	if ( sem->count >= 1 )
	{
		dv_relinquish_mutex(sem, kvars->current_thread);
	}

	return dv_eid_None;
}
