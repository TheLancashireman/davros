/*	dv-arm-returntothread.s - ARM thread resumption.
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
#include <kernel/h/dv-asm.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <cpufamily/arm/h/dv-arm-registers.h>

	.globl	dv_arm_return_to_thread

	.text

/* dv_arm_return_to_thread(dv_registers_t *r) - dispatch the thread whose register store is r
 *
 * Parameter r in r0
*/
dv_arm_return_to_thread:

#if 0
	/* restore FPU context */
	ldrb	r2, [r0, #DV_ARM_THR_FPUENABLED]	/* r2 = regs->fpuEnabled; */
	add		r1, r0, #DV_ARM_THR_D			/* r1 = &(regs->d) */
	cmp		r2, #DV_THRFPU_ENABLE			/* if (r2 == MK_THRFPU_ENABLE) */
	vldmiaeq.64	r1!, { d0 - d15 }			/* { {d0-d15} = regs->d[0-15] }*/
#if DV_FPU == DV_FPU_VFP3D32
	vldmiaeq.64	r1, { d16 - d31 }			/* { {d16-d31} = regs->d[16-31] }*/
#endif
	ldr		r1, [r0, #DV_ARM_THR_FPSCR]		/* r1 = regs->fpscr */
	vmsr	fpscr, r1						/* fpscr = r1 */
#endif

	clrex									/* Clear exclusive monitor (reservation). */

	ldr		r1, [r0, #DV_OFFSET_regs_cpsr]	/* r1 = regs->cpsr */
	msr		spsr_cxsf, r1					/* spsr = r1 */
	add		r1, r0, #DV_OFFSET_regs_sp		/* r1 = &(regs->sp) */
	ldmia	r1, {sp, lr}^					/* { sp_usr, lr_usr } = regs->{sp, lr} */
	ldmia	r0, {r0-r12, pc}^				/* Return from exception */

	/* The above instruction loads the program counter so the following instruction
	   is never reached.
	*/
dv_arm_return_to_thread_stop:
	b		dv_arm_return_to_thread_stop
