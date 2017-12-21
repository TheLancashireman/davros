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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-coverage.h>

DV_COVDEF(sys_exit);

/* dv_sys_exit() - do nothing, but in the kernel
 *
 * This function implements the kernel side of the exit system call.
*/
void dv_sys_exit(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_thread_t *thr = kvars->current_thread;
	dv_executable_t *exe = thr->executable;

#if 0
	/* We'll need some stuff from the thread for later. The thread will either be idle or
	 * hold a different executable (or a different instance of the same executable) after
	 * dv_kill_executable_in_thread()
	*/
	dv_dllkey_t prio = dv_get_prio(thr);
	dv_machineword_t p0 = dv_get_p0(thr->regs);
	dv_machineword_t p1 = dv_get_p1(thr->regs);
#endif

	dv_kill_executable_in_thread(kvars, thr, exe);

#if 0
	if ( exe->exitfunc != DV_NULL )
	{
		exe->exitfunc(...);
	}
#endif
}

/* man-page-generation - to be defined
*/
