/*	dv-semaphore.h - semaphore structure for davros
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
#ifndef dv_semaphore_h
#define dv_semaphore_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>

#if !DV_ASM

/* Semaphore protocols
*/
enum dv_semaphoreprotocol_e
{
	dv_semaphore_none = 0,			/* No mechanism; semaphore has not been configured */
	dv_semaphore_fifo,				/* Classic P/V (wait/signal) semaphore with fifo queue */
	dv_semaphore_priority,			/* Classic P/V (wait/signal) semaphore with priority queue */
	dv_semaphore_deferredceiling,	/* Priority ceiling protocol - always has priority queue */
	dv_semaphore_immediateceiling,	/* Immediate priority ceiling protocol (like OSEK/AUTOSAR) - no queue */
	dv_semaphore_nprotocols
};
typedef enum dv_semaphoreprotocol_e dv_semaphoreprotocol_t;

/* String representations for info functions.
*/
#define DV_SEMAPHOREPROTOCOLS \
	[dv_semaphore_none]				= "dv_semaphore_none",				\
	[dv_semaphore_fifo]				= "dv_semaphore_fifo",				\
	[dv_semaphore_priority]			= "dv_semaphore_priority",			\
	[dv_semaphore_deferredceiling]	= "dv_semaphore_deferredceiling",	\
	[dv_semaphore_immediateceiling]	= "dv_semaphore_immediateceiling"

/* The structure representing a thread.
*/
struct dv_semaphore_s
{
	dv_doublylinkedlist_t exe_queue;	/* List of executables that are waiting for the semaphore */
	dv_thread_t *owner;					/* Current owner of the semaphore */
	dv_semaphore_t *link;				/* Next in the list of semaphores that are held by the thread. */
	dv_i32_t oldprio;					/* Priority of owner before taking the semaphore */
	dv_semaphoreprotocol_t protocol;	/* Protocol to use for this semaphore */
	dv_i32_t ceiling;					/* Ceiling priority */
	dv_i32_t count;						/* Counter */
};

/* dv_occupy_sempahore() - occupy a sempahore; push onto a thread's "taken" list, set owner, etc.
 *
 * The "taken" list is a simple LIFO list. Double linking is not needed.
*/
static inline void dv_occupy_semaphore(dv_semaphore_t *sem, dv_thread_t *thr)
{
	sem->owner = thr;
	sem->link = thr->semtaken;
	sem->oldprio = dv_get_prio(thr);
	thr->semtaken = sem;
}

static inline dv_errorid_t dv_wait_semfifo(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

static inline dv_errorid_t dv_wait_semqprio(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

static inline dv_errorid_t dv_wait_semdefceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	return dv_eid_NotImplemented;
}

/* dv_wait_semimmceil() - occupy an immediate priority ceiling protocol mutex
 *
 * Executables with higher base priority are not allowed to occupy.
*/
static inline dv_errorid_t dv_wait_semimmceil(dv_kernel_t *kvars, dv_semaphore_t *sem)
{
	/* It's an error if the caller's base priority is higher than the semaphore's ceiling priority.
	*/
	if ( kvars->current_thread->executable->baseprio > sem->ceiling )
		return dv_eid_SemaphoreCeilingTooLow;

	sem->count--;

	/* Queueing on a semaphore with immediate priority ceiling protocol should never happen.
	*/
	if ( sem->count < 0 )
	{
		sem->count++;
		return dv_eid_SemaphoreAlreadyOccupied;
	}

	/* Occupy the semaphore.
	*/
	dv_occupy_semaphore(sem, kvars->current_thread);

	/* Raise the priority of the caller to the ceiling.
	*/
	dv_set_prio(kvars->current_thread, sem->ceiling);

	return dv_eid_None;
}

#endif

#endif
