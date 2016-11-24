/*	dv-thread.h - thread structure for davros
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

#include <kernel/include/dv-kconfig.h>
#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-kernel-types.h>
#include <kernel/include/dv-doublylinkedlist.h>

#if !DV_ASM

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
	dv_registers_t *regs;				/* Registers. Placed first for ease of access in assembly language */
	dv_dllelement_t link;				/* Link entry in the thread queue */
	dv_executable_t *executable;		/* Current executable */
	dv_thread_t *parent;				/* Parent thread (for synchronous threads) */
	dv_jobqueue_t *jobqueue;			/* Job queue (if any) */
	dv_lock_t *locktaken;				/* Innermost lock occupied by thread. */
	dv_dllkey_t prio;					/* Current priority */
	dv_threadstate_t state;				/* Current state */
};

#endif

#endif
