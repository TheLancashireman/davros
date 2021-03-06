/*	dv-undef.s - ARM undef kernel entry
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
#include <kernel/h/dv-asm.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <cpufamily/arm/h/dv-arm-registers.h>

	.globl	dv_trap_undef
	.extern	dv_catch_undef

	.text

dv_trap_undef:
	cpsid	aif			/* Disable IRQ, FIQ and async exceptions */

						/* ToDo: switch to kernel page table */

/*	Load address of current thread's registers into sp.
 *  sp = kvars->current_thread->regs->gpr[0];
 *	Assumes gpr array is first in regs structure.
*/
	mrc		p15, 0, sp, c13, c0, 3						/* kvars */
	ldr		sp, [sp, #DV_OFFSET_kvars_current_thread]	/* current_thread */
	ldr		sp, [sp, #DV_OFFSET_thr_regs]				/* regs->gpr[0] */
	stmia	sp, {r0-r7}									/* save r0-r7 in regs structure */

	mrs		r0, spsr									/* saved mode */
	mov		r2, sp										/* kvars->current_thread->regs->gpr[0] */

	add		r4, r2, #DV_OFFSET_regs_gpr8				/* &kvars->current_thread->regs->gpr[8] */
	stmia	r4, {r8-r12}								/* save r8-r12 in regs structure */
	str		r0, [r2, #DV_OFFSET_regs_cpsr]
	str		lr, [r2, #DV_OFFSET_regs_pc]				/* ToDo: adjustment of return address for undef trap */
	add		r4, r2, #DV_OFFSET_regs_sp
	stmia	r4, {sp,lr}^								/* save banked sp/lr */

	mrc		p15, 0, r0, c13, c0, 3						/* 1st param = kvars */
	ldr		sp, [r0, #DV_OFFSET_kvars_kernelsp]			/* kernel stack pointer */
	ldr		r1, [lr, #-4]								/* get the swi instruction */
	and		r1, r1, #0xff								/* 2nd param = syscall index */
	b		dv_catch_undef								/* go to trap handler */
