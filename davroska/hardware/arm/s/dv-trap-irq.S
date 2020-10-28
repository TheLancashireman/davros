/*	dv-trap-irq.S - ARM low-level irq handler
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
#define DV_ASM	1
#include <arm/h/dv-arm-registers.h>

	.globl	dv_trap_irq
	.extern	dv_catch_irq
	.extern dv_panic_failed_return_from_irq

	.text

/* dv_trap_irq() - low level IRQ handler
 *
 * sp_irq is initialised at startup to point to a context array where registers can be stored temporarily.
 * The sp_irq register is not used as a stack pointer, so we refer to it with its generic name r13 here.
 *
 * It is the responsibility of the irq handler to save the context array in an automatic variable before
 * re-enabling IRQ, and to restore the contents before returning.
 *	context[0] = r0
 *	context[1] = r1
 *	context[2] = r2
 *	context[3] = r3
 *	context[4] = r12
 *	context[5] = lr (pc of interrupted context) (dv_catch_irq needs to subtract 4)
 *	context[6] = spsr
 *	context[7] = lr_svc
*/

dv_trap_irq:
	stmia	r13, {r0-r3,r12,lr}	/* Save the volatile registers (r0-r3 and r12) and exception pc in the context array */
	mrs		r0, spsr			/* Get spsr */
	str		r0, [r13,#24]		/* Save spsr in context array */
	mov		r1, r13				/* Transfer r13 over mode switch */

	mrs		r12, cpsr			/* Switch to svc mode: */
	bic		r12, r12, #DV_ARM_MODE		/* Clear all mode bits */
	orr		r12, r12, #DV_ARM_MODE_SVC	/* Set svc mode */
	msr		cpsr_c, r12

	str		lr, [r1,#28]		/* Save lr_svc in context array */

	bl		dv_catch_irq		/* Handle the interrupt */

	ldr		r1, =dv_irqstack
	ldr		lr, [r1,#28]		/* Restore lr_svc from context array */

	mrs		r12, cpsr			/* Switch to irq mode: */
	bic		r12, r12, #DV_ARM_MODE		/* Clear all mode bits */
	orr		r12, r12, #DV_ARM_MODE_IRQ	/* Set svc mode */
	msr		cpsr_c, r12

	ldr		r0, [r13,#24]       /* Recover spsr */
	msr		spsr, r0
	ldmia	r13, {r0-r3,r12, pc}^	/* Restore all saved registers including PC and spsr->cpsr */

	b		dv_panic_failed_return_from_irq
