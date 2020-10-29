/*	dv-sysresume.c - resume system call for davros
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
#include <kernel/h/dv-executable.h>
#include DV_H_REGISTERS

DV_COVDEF(sys_resume);

/* dv_sys_resume() - wake up an executable from suspended animation
 *
 * This function implements the kernel side of the resume system call.
*/
void dv_sys_resume(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_machineword_t p0 = dv_get_p0(kvars->current_thread->regs);
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_index_t exe_i = (dv_index_t)p0;
	dv_executable_t *exe;
	dv_errorid_t e = dv_eid_UnknownError;

	DV_DBG(dv_kprintf("dv_sys_resume(): exe_i = %d\n", exe_i));
	if ( exe_i < 0 || exe_i >= dv_coreconfigs[kvars->core_index]->n_executables )
	{
		e = dv_eid_IndexOutOfRange;
		DV_DBG(dv_kprintf("dv_sys_resume(): e = %d (IndexOutOfRange)\n", e));
	}
	else
	{
		exe = &exe_tbl[exe_i];

		if ( exe->state == dv_exe_suspended )
		{
			e = dv_resume_executable(kvars, exe);
			DV_DBG(dv_kprintf("dv_sys_resume(): e = %d (returned from dv_resume_executable())\n", e));
		}
	    else if ( exe->state == dv_exe_free )
	        e = dv_eid_UnconfiguredExecutable;
	    else if ( exe->state == dv_exe_disabled )
	        e = dv_eid_ExecutableQuarantined;
	    else
	        e = dv_eid_ExecutableNotSuspended;
		
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}
