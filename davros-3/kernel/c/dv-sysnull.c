/*	dv-sysnull.c - null system call for davros
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

DV_COVDEF(sys_null);

/* dv_sys_null() - do nothing, but in the kernel
 *
 * This function implements the kernel side of the null system call.
 * The system call is intended as a debugging aid. The debug variant dumps the
 * calling thread's state.
*/
#if DV_DEBUG
void dv_sys_null(dv_kernel_t *kvars, dv_index_t sci)
{
	dv_kprintf("dv_sys_null() on core %d, sci = %d\n", kvars->core_index, sci);

	dv_kprintf(" -- kvars = 0x%08x, current_thread = 0x%08x, executable = 0x%08x\n",
						kvars, kvars->current_thread, kvars->current_thread->executable);
	dv_trace_dumpregs(kvars->current_thread->executable->name, kvars->current_thread->regs);
}



#else
void dv_sys_null(dv_kernel_t *unused_kvars, dv_index_t unused_sci)
{
}
#endif
