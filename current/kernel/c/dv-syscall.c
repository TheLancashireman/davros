/*	dv-syscall.c - system-call handler for davros
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
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(dv_syscall);

dv_sc_function_t const dv_syscalltable[DV_N_SYSCALL+1] = { DV_SYSCALLTABLE };

/* dv_function() - short description
 *
 * Long description
*/
void dv_syscall(dv_kernel_t *kvars, dv_machineword_t sc_index)
{
	dv_fcov(0);

	if ( dv_ccov(1, 2, dv_dllisempty(&kvars->thread_queue)) )
	{
		dv_panic(dv_panic_threadqueueempty, "dv_syscall", "Thread queue is empty");
	}
	else
	if ( dv_ccov(3, 4, (kvars->current_thread == dv_threadqueuehead(kvars))) )
	{
		if ( dv_ccov(5, 6, (sc_index < (dv_machineword_t)DV_N_SYSCALL)) )
		{
			dv_identity_t sci = (dv_identity_t)sc_index;
			(*dv_syscalltable[sci])(kvars, sci);
		}
		else
		{
			dv_sys_unknown(kvars, (dv_identity_t)sc_index);
		}

		dv_dispatch(kvars);
	}
	else
	{
		dv_panic(dv_panic_currentthreadnotqueuehead, "dv_syscall", "Current thread is not at head of queue");
	}
}

/* man-page-generation - to be defined
*/
