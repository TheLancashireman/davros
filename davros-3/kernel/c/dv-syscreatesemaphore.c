/*	dv-syscreatesemaphore.c - create_semaphore system call for davros
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
#include <kernel/h/dv-semaphore.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-doublylinkedlist.h>

DV_COVDEF(sys_create_semaphore);

/* dv_is_free_sem() - retrun true if semaphore i is free
*/
static dv_boolean_t dv_is_free_sem(dv_index_t i, const void *tbl)
{
	const dv_semaphore_t *sem_tbl = tbl;
	if ( sem_tbl[i].protocol == dv_semaphore_none )
		return 1;
	return 0;
}


/* dv_sys_create_semaphore() - create a semaphore
 *
 * This function implements the kernel side of the create_semaphore system call.
*/
void dv_sys_create_semaphore(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_semaphore_t *sem_tbl = dv_coreconfigs[kvars->core_index]->semaphores;
	dv_quantity_t n_sem = dv_coreconfigs[kvars->core_index]->n_semaphores;

	char *name = (char *)dv_get_p0(kvars->current_thread->regs);
	dv_semaphoreprotocol_t protocol = (dv_semaphoreprotocol_t)dv_get_p1(kvars->current_thread->regs);
	dv_i32_t count = (dv_i32_t)dv_get_p2(kvars->current_thread->regs);
	dv_errorid_t e = dv_eid_UnknownError;

	if ( protocol <= dv_semaphore_none || protocol >= dv_semaphore_nprotocols )
		e = dv_eid_UnknownSemaphoreProtocol;
	else if ( protocol == dv_semaphore_fifo || protocol == dv_semaphore_priority || count == 1 )
	{
		dv_index_t sem_i = dv_allocate_obj(&kvars->sem_allocator, n_sem, dv_is_free_sem, sem_tbl);

		if ( sem_i < 0 )
		{
			dv_panic(dv_panic_objectsearchfailed, "dv_sys_create_semaphore", "no free semaphore found");
		}
		else
		{
			dv_semaphore_t *sem = &sem_tbl[sem_i];

			sem->name = name;
			dv_dllinit(&sem->exe_queue, dv_dll_priority);
			sem->owner = DV_NULL;
			sem->link = DV_NULL;
			sem->oldprio = DV_MIN_PRIORITY;
			sem->ceiling = DV_MIN_PRIORITY;
			sem->protocol = protocol;
			sem->count = count;

			e = dv_eid_None;
			dv_set_rv1(kvars->current_thread->regs, sem_i);
		}
	}
	else
		e = dv_eid_InvalidCountForMutex;

	dv_set_rv0(kvars->current_thread->regs, e);
}
