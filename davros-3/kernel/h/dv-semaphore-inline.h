/*	dv-semaphore-inline.h - inline semaphore functions for davros
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
#ifndef dv_semaphore_inline_h
#define dv_semaphore_inline_h	1

#include <kernel/h/dv-semaphore.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-kernel-types.h>

#if !DV_ASM

/* dv_acquire_mutex() - acquire a mutex sempahore; push onto executable's "taken" list, set owner, etc.
 *
 * The "taken" list is a simple LIFO list. Double linking is not needed.
*/
static inline void dv_acquire_mutex(dv_semaphore_t *sem, dv_thread_t *thr)
{
	dv_executable_t *exe = thr->executable;
	sem->owner = exe;
	sem->link = exe->semtaken;
	exe->semtaken = sem;
	sem->oldprio = dv_get_prio(thr);
}

/* dv_relinquish_sempahore() - relinquish a sempahore; pop the thread's "taken" list, clear owner, etc.
 *
 * The "taken" list is a simple LIFO list. Double linking is not needed.
*/
static inline void dv_relinquish_mutex(dv_semaphore_t *sem, dv_thread_t *thr)
{
	sem->owner = DV_NULL;
	thr->executable->semtaken = sem->link;
	sem->link = DV_NULL;
	dv_set_prio(thr, sem->oldprio);
	dv_dlldemote(&thr->link);
}

/* dv_wait_semfifo() - wait for a fifo semaphore
 *
*/
static inline dv_errorid_t dv_wait_semfifo(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_signal_semfifo() - signal a fifo semaphore
 *
*/
static inline dv_errorid_t dv_signal_semfifo(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_wait_semfifo() - wait for a priority semaphore
 *
*/
static inline dv_errorid_t dv_wait_semqprio(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_signal_semfifo() - signal a priority semaphore
 *
*/
static inline dv_errorid_t dv_signal_semqprio(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_wait_semdefceil() - acquire a mutex with deferred ceiling priority protocol
 *
*/
static inline dv_errorid_t dv_wait_semdefceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_signal_semdefceil() - relinquish a mutex with deferred ceiling priority protocol
 *
*/
static inline dv_errorid_t dv_signal_semdefceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

#endif

#endif
