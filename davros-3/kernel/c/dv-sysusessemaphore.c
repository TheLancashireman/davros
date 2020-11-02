/*	dv-sysusessemaphore.c - uses semaphore system call for davros
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

DV_COVDEF(sys_uses_semaphore);

/* dv_sys_uses_semaphore() - uses a semaphore
*/
void dv_sys_uses_semaphore(dv_kernel_t *kvars, dv_index_t unused_sci)
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
		else
		{
			dv_index_t exe_i = (dv_index_t)dv_get_p1(kvars->current_thread->regs);
			dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
			dv_executable_t *exe;

			if ( exe_i < 0 || exe_i >= dv_coreconfigs[kvars->core_index]->n_executables )
				e = dv_eid_IndexOutOfRange;
			else
			{
				exe = &exe_tbl[exe_i];

				if ( exe->state == dv_exe_free )
					e = dv_eid_UnconfiguredExecutable;
				else
				{
					e = dv_eid_None;

					if ( sem->ceiling < exe->baseprio )
						sem->ceiling = exe->baseprio;
				}
			}
		}
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}
