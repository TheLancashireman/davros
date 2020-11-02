/*	dv-sysdestroysemaphore.c - destroy semaphore system call for davros
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
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-doublylinkedlist.h>

DV_COVDEF(sys_destroy_semaphore);

/* dv_sys_destroy_semaphore() - destroy a semaphore
*/
void dv_sys_destroy_semaphore(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_index_t sem_i = (dv_index_t)dv_get_p0(kvars->current_thread->regs);
	dv_semaphore_t *sem_tbl = dv_coreconfigs[kvars->core_index]->semaphores;
	dv_semaphore_t *sem;
	dv_errorid_t e = dv_eid_UnknownError;

	if ( sem_i < 0 || sem_i >= dv_coreconfigs[kvars->core_index]->n_semaphores )
	{
		e = dv_eid_IndexOutOfRange;
	}
	else
	{
		sem = &sem_tbl[sem_i];

		if ( sem->protocol == dv_semaphore_none )
			e = dv_eid_UnconfiguredSemaphore;
		else if ( sem->owner == DV_NULL )
		{
			/* ToDo: release all the waiting executables.
			*/
			if ( dv_dllisempty(&sem->exe_queue) )
			{
				sem->name = DV_NULL;
				dv_dllinit(&sem->exe_queue, dv_dll_priority);
				sem->link = DV_NULL;
				sem->oldprio = DV_MIN_PRIORITY;
				sem->ceiling = DV_MIN_PRIORITY;
				sem->protocol = dv_semaphore_none;
				sem->count = 0;
				dv_deallocate_obj(&kvars->sem_allocator);

				e = dv_eid_None;
			}
			else
				e = dv_eid_SemaphoreOccupied;
		}
		else
			e = dv_eid_SemaphoreOccupied;
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}
