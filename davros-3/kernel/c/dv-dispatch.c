/*	dv-dispatch.c - thread dispatcher for davros
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
#include <dv-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-trace.h>
#include DV_H_DISPATCH

DV_COVDEF(dispatch);

#if DV_EXECUTIONBUDGET

#error "execution budget not implemented"


static inline dv_thread_t *dv_select_most_eligible(kvars)
{
	return dv_threadqueuehead(kvars);
}

static inline void dv_set_exectimer(dv_thread_t *unused_incoming)
{
	/* ToDo: implement this */
}

static inline void dv_adjust_exectimer(dv_threat_t *unused_incoming)
{
	/* Intentionally left blank */
}

#else

/* dv_select_most_eligible() - select the most eligible thread from the thread queue.
 *
 * No execution budgets, so the most eligible is simply the head of the queue.
*/
static inline dv_thread_t *dv_select_most_eligible(dv_kernel_t *kvars)
{
	return dv_threadqueuehead(kvars);
}

/* dv_set_exectimer() - start or stop the exectimer.
 *
 * No execution budgets, so this function does nothing.
*/
static inline void dv_set_exectimer(dv_thread_t *unused_incoming)
{
	/* Intentionally left blank */
}

static inline void dv_adjust_exectimer(dv_thread_t *unused_incoming)
{
	/* Intentionally left blank */
}

#endif

/* dv_dispatch() - thread dispatcher
 *
 * The thread dispatcher determines the most eligible thread and switches to it.
*/
void dv_dispatch(dv_kernel_t *kvars)
{
	dv_thread_t *outgoing = kvars->current_thread;
	dv_thread_t *incoming = dv_select_most_eligible(kvars);

	DV_DBG(dv_kprintf("dv_dispatch(): outgoing = %s, incoming = %s\n",
						(outgoing == DV_NULL ? "<null>" : outgoing->executable->name), incoming->executable->name));

	/* ToDo: make the following an optional assertion.
	*/
	dv_assert( (incoming != DV_NULL),
		dv_panic_threadqueueempty, "dv_dispatch", "No thread to run");

	if ( outgoing == incoming )
	{
		dv_adjust_exectimer(incoming);
	}
	else
	{
		if ( outgoing != DV_NULL )
		{
			dv_trace_threadstate(outgoing, dv_thread_ready);
			outgoing->state = dv_thread_ready;
		}

		dv_trace_threadstate(incoming, dv_thread_running);
		incoming->state = dv_thread_running;
		kvars->current_thread = incoming;
		dv_set_exectimer(incoming);
	}

	/* ToDo: return and clear pending events */

	dv_set_runprio(incoming);

	dv_return_to_thread(kvars, incoming);		/* Never returns */
}
