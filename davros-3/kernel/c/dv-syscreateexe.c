/*	dv-syscreateexe.c - create_exe system call for davros
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
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-executable.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-coreconfig.h>

DV_COVDEF(sys_spawn);

/* dv_sys_create_exe() - create an executable
 *
 * This function implements the kernel side of the create_exe system call.
*/
void dv_sys_create_exe(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_machineword_t p0 = dv_get_p0(kvars->current_thread->regs);
	dv_errorid_t e = dv_eid_UnknownError;
	dv_index_t exe;

	/* ToDo: pointer parameter validation. For now, just check alignment
	*/
	if ( (p0 & 0x03) == 0 )
	{
		exe = dv_create_executable(kvars, (dv_execonfig_t *)p0);
		if ( exe < 0 )
			e = dv_eid_ExecutableCreationFailed;		/* ToDo: be more specific */
		else
		{
			e = dv_eid_None;
			dv_set_rv1(kvars->current_thread->regs, exe);
		}
	}
	else
		e = dv_eid_InvalidPointerParameter;
		

	dv_set_rv0(kvars->current_thread->regs, e);
}

/* man-page-generation - to be defined
*/
