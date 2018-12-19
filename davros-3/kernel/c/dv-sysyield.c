/*	dv-sysyield.c - yield system call for davros
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
#include <kernel/h/dv-syscall.h>

DV_COVDEF(sys_yield);

/* dv_sys_yield() - yield to executables with higher base priority
 *
 * This function implements the kernel side of the yield system call.
*/
void dv_sys_yield(dv_kernel_t *kvars, dv_index_t unused_sci)
{
	dv_thread_t *thr = kvars->current_thread;
	dv_thread_t *next = thr->link.successor->payload;

	/* ToDo: what if a lock is occupied? */

	if ( next->link.key.i32_key > thr->executable->baseprio )
	{
		dv_dllremove(&thr->link);
		dv_set_prio(thr, thr->executable->baseprio);
		dv_dllinsertaftersame(&kvars->thread_queue, &thr->link);
	}
}
