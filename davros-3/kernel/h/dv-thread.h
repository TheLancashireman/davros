/*	dv-thread.h - thread structure for davros
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
#ifndef dv_thread_h
#define dv_thread_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-executable.h>
#include DV_H_REGISTERS

#define DV_MIN_PRIORITY		(DV_DLLMINI32KEY+1)
#define DV_MAX_PRIORITY		(2147483647)

#if !DV_ASM

/* Thread states
*/
enum dv_threadstate_e
{
	dv_thread_idle = 0,
	dv_thread_running,
	dv_thread_ready,
	dv_thread_nstates
};
typedef enum dv_threadstate_e dv_threadstate_t;

/* String representations for info functions.
*/
#define DV_THREADSTATES \
	[dv_thread_idle]	= "dv_thread_idle",		\
	[dv_thread_running]	= "dv_thread_running",	\
	[dv_thread_ready]	= "dv_thread_ready"

/* The structure representing a thread.
*/
struct dv_thread_s
{
	dv_registers_t *regs;				/* Registers. Placed first for ease of access in assembly language */
	dv_dllelement_t link;				/* Link entry in the thread queue */
	dv_executable_t *executable;		/* Current executable */
	dv_thread_t *parent;				/* Parent thread (for synchronous threads) */
	dv_ringbuffer_t *jobqueue;			/* Job queue (if any) */
	dv_semaphore_t *semtaken;			/* Innermost semaphore occupied by thread. */
	dv_threadstate_t state;				/* Current state */
	dv_i32_t current_prio;				/* Current priority */
	dv_i32_t n_exe;						/* No. of executables using this thread. */
};

/* dv_set_runprio() - set a thread's running priority
 *
 * Set the thread's priority to the executable's running priority, unless already greater.
*/
static inline void dv_set_runprio(dv_thread_t *thr)
{
	if ( thr->link.key.i32_key < thr->executable->runprio )
	{
		thr->link.key.i32_key = thr->executable->runprio;
	}
}

/* dv_get_prio() - get a thread's priority
*/
static inline dv_i32_t dv_get_prio(dv_thread_t *thr)
{
	return (thr->link.key.i32_key);
}

/* dv_set_prio() - set a thread's priority
*/
static inline void dv_set_prio(dv_thread_t *thr, dv_i32_t p)
{
	thr->link.key.i32_key = p;
}

/* dv_thread_rb_length() - calculate required ringbuffer length
*/
static inline dv_i32_t dv_thread_rb_length(dv_i32_t n)
{
	/* Ensure that the length is at least the minumum.
	*/
	if ( n < DV_MIN_THREADRB )
		return DV_MIN_THREADRB;

	/* Increase to the next multiple of the rounding config.
	*/
	return n + DV_ROUND_THREADRB - n % DV_ROUND_THREADRB;
}

extern dv_errorid_t dv_enqueue_job_in_jobqueue(dv_kernel_t *, dv_executable_t *);
extern void dv_return_from_main(dv_machineword_t, dv_machineword_t);

#endif

#define DV_OFFSET_thr_regs		0

#endif
