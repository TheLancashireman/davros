/*	dv-wakeup.c - wake up sleeping executables
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
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include DV_H_SYSTEMTIMER

/* ToDo: job queues.
*/
#define dv_enqueue_job_in_jobqueue(x, y) \
	dv_panic(dv_panic_unimplemented, "dv_wakeup", "jobqueue not implemented")

DV_COVDEF(wakeup);

/* dv_wakeup() - wake up sleeping executables
 *
 * This function wakes up all sleeping executables whose time has come, and
 * does anything else that needs doing too :-)
*/
dv_u64_t dv_wakeup(dv_kernel_t *kvars)
{
	dv_executable_t *exe;

	for (;;)
	{
		dv_u64_t now = dv_readtime();
		dv_dllelement_t *first = kvars->sleep_queue.headtail.successor;

		if ( first->key.u64_key == 0xffffffffffffffff )
		{
			/* Sleep queue is empty
			*/
			return now + 1000000;		/* ToDo: use a constant for 1 second */
		}

		if ( first->key.u64_key <= (now + DV_MIN_SLEEP) )
		{
			dv_dllremove(first);

			switch ( first->payload_type )
			{
			case dv_dll_exe:
				exe = (dv_executable_t *)first->payload;
				exe->state = dv_exe_active;
				exe->dll_element->key.u64_key = 0;
				if ( exe->thread->state == dv_thread_idle )
					dv_resume_executable_in_thread(&kvars->thread_queue, exe, exe->thread);
				else
					dv_enqueue_job_in_jobqueue(exe->thread, exe);
				break;

				/* ToDo: other payload_type cases as required
				*/

			default:
				dv_panic(dv_panic_unimplemented, "dv_wakeup", "incorrect payload type for sleep queue");
				break;
			}
		}
		else
		{
			return first->key.u64_key;
		}
	}
}
