/*	dv-syswait.c - wait system call for davros
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-semaphore.h>
#include DV_H_REGISTERS

DV_COVDEF(sys_wait);

/* dv_sys_wait() - wait for a semaphore
 *
 * This function implements the kernel side of the wait system call.
*/
void dv_sys_wait(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_machineword_t p0 = dv_get_p0(kvars->current_thread->regs);
	dv_semaphore_t *sem_tbl = dv_coreconfigs[kvars->core_index]->semaphores;
	dv_index_t sem_i = (dv_index_t)p0;
	dv_semaphore_t *sem;
	dv_errorid_t e = dv_eid_UnknownError;

	DV_DBG(dv_kprintf("dv_sys_wait(): sem_i = %d\n", sem_i));
	if ( sem_i < 0 || sem_i >= dv_coreconfigs[kvars->core_index]->n_semaphores )
	{
		e = dv_eid_IndexOutOfRange;
		DV_DBG(dv_kprintf("dv_sys_wait(): e = %d (IndexOutOfRange)\n", e));
	}
	else
	{
		sem = &sem_tbl[sem_i];

		switch ( sem->protocol )
		{
		case dv_semaphore_fifo:
			e = dv_wait_semfifo(kvars, sem);
			break;
		case dv_semaphore_priority:
			e = dv_wait_semqprio(kvars, sem);
			break;
		case dv_semaphore_deferredceiling:
			e = dv_wait_semdefceil(kvars, sem);
			break;
		case dv_semaphore_immediateceiling:
			e = dv_wait_semimmceil(kvars, sem);
			break;
		default:
			e = dv_eid_SemaphoreUnconfigured;
			break;
		}

		DV_DBG(dv_kprintf("dv_sys_wait(): e = %d (returned from dv_wait_***())\n", e));
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}
