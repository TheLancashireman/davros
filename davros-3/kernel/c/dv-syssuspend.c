/*	dv-syssuspend.c - suspend system call for davros
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
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include DV_H_REGISTERS

DV_COVDEF(sys_suspend);

/* dv_sys_suspend() - put a task into suspended animation
 *
 * This function implements the kernel side of the suspend system call.
 * The calling executable is placed into a suspended state. It gets woken up again by a call to dv_resume()
*/
void dv_sys_suspend(dv_kernel_t *kvars, dv_index_t sci)
{
	dv_errorid_t e = dv_eid_UnknownError;
	dv_executable_t *exe;

	exe = kvars->current_thread->executable;

	if ( (exe->flags & DV_EXEFLAG_BLOCKING) == 0 )
	{
		e = dv_eid_ExecutableIsNonBlocking;
		DV_DBG(dv_kprintf("dv_sys_suspend(): e = %d (ExecutableIsNonBlocking)\n", e));
	}
	else if ( exe->semtaken != DV_NULL )
	{
		e = dv_eid_ExecutableOccupiesSemaphore;
		DV_DBG(dv_kprintf("dv_sys_suspend(): e = %d (ExecutableOccupiesSemaphore)\n", e));
	}
	else
	{
		e = dv_eid_None;
		dv_remove_executable_from_thread(kvars, kvars->current_thread);
		exe->state = dv_exe_suspended;
	}

	dv_set_rv0(exe->registers, e);
}
