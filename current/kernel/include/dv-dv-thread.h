/*	dv-thread.h - threads for davros
 *
 * A *thread* is a mechanism by which davros manages all the currently active
 * executables.
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
#ifndef dv_thread_h
#define dv_thread_h	1

#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-kernel-types.h>

/* Thread states
*/
enum dv_threadstate_e
{
	dv_thread_idle	= 0,
	dv_thread_running,
	dv_thread_ready,
	dv_thread_new
};
typedef enum dv_threadstate_e dv_threadstate_t;

/* The structure representing a thread.
*/
struct dv_thread_s
{
	dv_registers_t *regs;				/* Pointer to the register store */
	dv_dllelement_t link;				/* Embedded entry for queueing */
	dv_thread_t *parent;				/* Parent thread (for synchronous threads) */
	dv_executable_t *currentExe;		/* What the thread is currently running */
	dv_threadprio_t currentPriority;	/* Current thread priority */
	dv_threadstate_t state;				/* State of thread (idle, ready, running) */
#if 0
	dv_resource_t *lastLockTaken; 		/* Most recently taken resource */
	dv_jobqueue_t *jobQueue;			/* Job queue (could be NULL) */
	dv_objectid_t memoryPartition;		/* Index of memory partition */
#endif
};

#endif
