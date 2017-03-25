/*	dv-sysnull.c - null system call for davros
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
#include <kernel/h/dv-coverage.h>

/* Todo: move the register dump function to cpu-specific and call it here.
*/
#include DV_REGISTERS

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
	dv_registers_t *r;
	int i;
	dv_kprintf("dv_sys_null() on core %d, sci = %d\n", kvars->core_index, sci);

	r = kvars->current_thread->regs;

	for ( i=0; i<13; i++ )
	{
		dv_kprintf("r%-2d  = 0x%08x\n", i, r->gpr[i]);
	}
	dv_kprintf("pc   = 0x%08x\n", r->pc);
	dv_kprintf("cpsr = 0x%08x\n", r->cpsr);
	dv_kprintf("sp   = 0x%08x\n", r->sp);
	dv_kprintf("lr   = 0x%08x\n", r->lr);
}
#else
void dv_sys_null(dv_kernel_t *unused_kvars, dv_index_t unused_sci)
{
}
#endif

/* man-page-generation - to be defined
*/
