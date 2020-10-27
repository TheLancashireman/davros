/*	dv-sysexit.c - exit system call for davros
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
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-syscall.h>

DV_COVDEF(sys_exit);

/* dv_sys_exit() - do nothing, but in the kernel
 *
 * This function implements the kernel side of the exit system call.
*/
void dv_sys_exit(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_thread_t *thr = kvars->current_thread;
	dv_executable_t *exe = thr->executable;

	dv_terminate_executable_in_thread(kvars, thr);

	if ( exe->flags & DV_EXEFLAG_AUTODESTROY )
		dv_destroy_executable(kvars, exe);

#if 0	/* ToDo: call exit function */
	dv_i32_t prio = dv_get_prio(thr);
	dv_machineword_t p0 = dv_get_p0(thr->regs);
	dv_machineword_t p1 = dv_get_p1(thr->regs);

	if ( exe->exitfunc != DV_NULL )
	{
		exe->exitfunc(...);
	}
#endif
}
