/*	dv-sysspawn.c - spawn system call for davros
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
#include DV_REGISTERS
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-stdio.h>

DV_COVDEF(sys_spawn);

/* dv_sys_spawn() - spawn an executable
 *
 * This function implements the kernel side of the spawn and spawn_async system calls.
*/
void dv_sys_spawn(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_machineword_t p0 = dv_get_p0(kvars->current_thread->regs);
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_index_t exe_i = (dv_index_t)p0;
	dv_executable_t *exe;
	dv_errorid_t e = dv_nerrors;
	
	DV_DBG(dv_kprintf("dv_sys_spawn(): exe_i = %d\n", exe_i));
	if ( exe_i < 0 || exe_i >= dv_coreconfigs[kvars->core_index]->n_executables )
	{
		e = dv_eid_IndexOutOfRange;
		DV_DBG(dv_kprintf("dv_sys_spawn(): e = %d (IndexOutOfRange)\n", e));
	}
	else
	{
		exe = &exe_tbl[exe_i];

		if ( exe->name == DV_NULL )
		{
			e = dv_eid_UnconfiguredExecutable;
			DV_DBG(dv_kprintf("dv_sys_spawn(): e = %d (UnconfiguredExecutable)\n", e));
		}
		else
		{
			e = dv_spawn_executable(kvars, exe);
			DV_DBG(dv_kprintf("dv_sys_spawn(): e = %d (returned from dv_spawn_executable())\n", e));
		}
	}

	dv_set_rv0(kvars->current_thread->regs, e);
}

/* man-page-generation - to be defined
*/
