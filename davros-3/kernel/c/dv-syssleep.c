/*	dv-syssleep.c - sleep system call for davros
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
#include <kernel/h/dv-doublylinkedlist.h>
#include DV_H_REGISTERS
#include DV_H_SYSTEMTIMER

DV_COVDEF(sys_sleep);

/* dv_sys_sleep() - put a task to sleep
 *
 * This function implements the kernel side of the sleep system call.
 * The calling executable is placed into a sleeping state and woken up again after n ticks of the system timer.
*/
void dv_sys_sleep(dv_kernel_t *kvars, dv_index_t sci)
{
	dv_machineword_t p0 = dv_get_p0(kvars->current_thread->regs);
	dv_errorid_t e = dv_eid_UnknownError;
	dv_executable_t *exe;

	exe = kvars->current_thread->executable;

	if ( exe->dll_element == DV_NULL )
	{
		e = dv_eid_ExecutableIsNonBlocking;
		DV_DBG(dv_kprintf("dv_sys_sleep(): e = %d (ExecutableIsNonBlocking)\n", e));
	}
	else
	if ( p0 >= DV_MIN_SLEEP )
	{
		e = dv_eid_None;
		dv_remove_executable_from_thread(kvars, kvars->current_thread);
		exe->state = dv_exe_sleep;

		dv_u64_t t = exe->dll_element->key.u64_key = dv_readtime() + p0;

		if ( dv_dllinserttime(&kvars->sleep_queue, exe->dll_element) )
		{
			/* Inserted at queue head; need to set the timer interrupt.
			*/
			dv_set_system_timer_alarm(t);
		}
	}
	else
	{
		e = dv_eid_TimeInThePast;
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}

/* man-page-generation - to be defined
*/
