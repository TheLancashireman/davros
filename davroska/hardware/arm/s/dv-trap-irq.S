/*	dv-trap-irq.s - ARM low-level irq handler
 *
 *	Copyright 2018 David Haworth
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
 * sp_irq is initialised at startup to point to an array where registers can be stored temporarily.
 * The sp_irq register is not used as a stack pointer, so we refer to it with its generic name r13 here.
 *
 * It is the responsibility of the irq handler to save the array in an automatic variable before re-enabling IRQ,
 * and to restore the contents before returning.
 *	array[0] = r0
 *	array[1] = r1
 *	array[2] = r2
 *	array[3] = r3
 *	array[4] = r12
 *	array[5] = pc (lr - 4)
 *	array[6] = spsr
*/

dv_trap_irq:
	stmia	r13, {r0-r3,r12}	/* Save the volatile registers (r0-r3 and r12) and exception pc in the irq array */
	sub		r3, lr, #4			/* IRQ needs a 4-byte adjustment */
	str		r3, [r13,#20]		/* Save adusted lr in array */
	mrs		r0, spsr			/* Get spsr */
	str		r0, [r13,#24]		/* Save spsr in array */

	mrs		r12, cpsr			/* Switch to svc mode: */
	bic		r12, r12, #DV_ARM_MODE		/* Clear all mode bits */
	orr		r12, r12, #DV_ARM_MODE_SVC	/* Set svc mode */
	msr		cpsr_c, r12

	bl		dv_catch_irq		/* Handle the interrupt */

	mrs		r12, cpsr			/* Switch to irq mode: */
	bic		r12, r12, #DV_ARM_MODE		/* Clear all mode bits */
	orr		r12, r12, #DV_ARM_MODE_IRQ	/* Set svc mode */
	msr		cpsr_c, r12

	ldr		r0, [r13,#24]       /* Recover spsr */
	msr		spsr, r0
	ldmia	r13, {r0-r3,r12, pc}^	/* Restore all saved registers including PC and spsr->cpsr */

	b		dv_panic_failed_return_from_irq
