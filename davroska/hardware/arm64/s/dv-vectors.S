/*	dv-vectors.s - ARM64 exception and interrupt vectors for davroska
 *
 *	Copyright 2019 David Haworth
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
	.globl		dv_vectortable

	.extern		dv_catch_fiq
	.extern		dv_catch_fiq_wrong_state

	.extern		dv_catch_irq
	.extern		dv_catch_irq_wrong_state

	.extern		dv_catch_synchronous_exception
	.extern		dv_catch_synchronous_exception_wrong_state

	.extern		dv_catch_syserror
	.extern		dv_catch_syserror_wrong_state

/* dv_vectortable - a standard ARM64 vector table
 *     
 * On arm-v8 architecture there are three major vector tables, one for each exception level
 * that the interrupt or exception is targeted at: EL1, EL2 and EL3. Interrupts and exceptions
 * cannot be targeted at EL0.
 * The base addresses of these three major tables are held in the registers VBAR_EL1, VBAR_EL2 and VBAR_EL3.
 *
 * Within each major vector table, there are four minor tables, one for each processor state that the
 * exception comes from:
 *	- same EL with SP0
 *	- same EL with SPx
 *	- lower EL using aarch64
 *	- lower EL using aarch32
 *
 * Each minor vector table contains four vectors: Synchronous, IRQ, FIQ and SError. Each vector is
 * 0x80 bytes long (32 instructions), so a minor table of four vectors occupies 0x200 bytes.
 *
 * Davros runs in EL1 with SP1, so it puts the address of its vector table in VBAR_EL1.
 * Privileged threads run at EL1 with SP0. Non-privileged threads run at EL0 with SP0.
 *
 * This means that:
 *	- an exception from a non-privileged thread uses the vectors at VBAR_EL1+0x400
 *	- an exception from a privileged thread uses the vectors at VBAR_EL1+0x000
 *	- an exception from the kernel uses the vectors at VBAR_EL1+0x200
 *	- an exception from an aarch32 thread (if supported) would use the vectors at VBAR_EL1+0x600
 *
 * From a system point of view (depending on what's running outside davros) it might be necessry
 * to have a stub EL2 and maybe EL3 handler to catch anything that accidentally goes to a higher EL.
*/

/* A macro to save the scratch registers to the current stack
 *
 * This macro is used when the exception comes from a thread (SP0).
 * If we use it when the exception comes from the kernel, we trash the thread's registers.
 * That's OK for starters :-)
 *
 * This macro must not be more than 31 instructions.
*/
	.macro	dv_exc_save_scratch
	sub		sp, sp, #0x100				/* A few slots spare ... */
	stp		x0, x1, [sp, #0x00]
	stp		x2, x3, [sp, #0x10]
	stp		x4, x5, [sp, #0x20]
	stp		x6, x7, [sp, #0x30]
	stp		x8, x9, [sp, #0x40]
	stp		x10, x11, [sp, #0x50]
	stp		x12, x13, [sp, #0x60]
	stp		x14, x15, [sp, #0x70]
	stp		x16, x17, [sp, #0x80]
	stp		x18, x30, [sp, #0x90]
	mrs		x0, ELR_EL1					/* PC where exception occurred */
	mrs		x1, SPSR_EL1				/* Saved PSTATE */
	stp		x0, x1, [sp, #0xa0]
	mrs		x2, SP_EL0					/* Thread SP */
	mrs		x3, ESR_EL1					/* Exception syndrome */
	stp		x2, x3, [sp, #0xb0]			/* These two might be useful in the exception handler */
	mrs		x4, FPCR					/* Floating point control */
	mrs		x5, FPSR					/* Floating point status */
	stp		x4, x5, [sp, #0xc0]
	mov		x0, sp						/* Pass address of this lot to the exception catcher */
	.endm

/* Now the EL1 vector table
*/
	.text
	.balign	4096				/* Page boundary */
dv_vectortable:

/* ============================================
 * Exceptions from privileged thread (EL1, SP0)
 * ============================================
*/
	.balign	128
dv_exc_priv_sync:				/* Synchronous exception from privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_synchronous_exception_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_priv_irq:				/* Irq from privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_irq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_priv_fiq:				/* Fiq from privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_fiq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_priv_serror:				/* SError from privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_syserror_wrong_state
	b		dv_exception_return

/* =================================
 * Exceptions from kernel (EL1, SP1)
 * =================================
*/
	.balign	128
dv_exc_kernel_sync:				/* Synchronous exception from kernel */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_synchronous_exception
	b		dv_exception_return

	.balign	128
dv_exc_kernel_irq:				/* Irq exception from kernel */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	b		dv_irq_save_simd

	.balign	128
dv_exc_kernel_fiq:				/* Fiq exception from kernel */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_fiq
	b		dv_exception_return

	.balign	128
dv_exc_kernel_serror:			/* SError from kernel */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_syserror
	b		dv_exception_return

/* ================================================
 * Exceptions from non-privileged thread (EL0, SP0)
 * ================================================
*/
	.balign	128
dv_exc_nonpriv_sync:			/* Synchronous exception from non-privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_synchronous_exception_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_nonpriv_irq:				/* Irq from non-privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_irq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_nonpriv_fiq:				/* Fiq from non-privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_fiq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_nonpriv_serror:			/* SError from non-privileged thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_syserror_wrong_state
	b		dv_exception_return

/* ==============================================
 * Exceptions from aarch32 thread
 * ==============================================
*/
	.balign	128
dv_exc_aa32_sync:				/* Synchronous exception from aarch32 thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_synchronous_exception_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_aa32_irq:				/* Irq from aarch32 thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_irq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_aa32_fiq:				/* Fiq from aarch32 thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_fiq_wrong_state
	b		dv_exception_return

	.balign	128
dv_exc_aa32_serror:				/* SError from aarch32 thread */
	dv_exc_save_scratch			/* Save registers to thread (macro) */
	bl		dv_catch_syserror_wrong_state
	b		dv_exception_return

/* ==============================================
 * dv_irq_save_restore_simd()
 *	- saves the SIMD registers on the stack
 *	- calls the irq catcher
 *	- restores the SIMD
 *	- falls through into the exception return
 * ==============================================
*/
dv_irq_save_simd:
	sub		sp, sp, #0x200
	stp		q0, q1, [sp, 0x000]
	stp		q2, q3, [sp, 0x020]
	stp		q4, q5, [sp, 0x040]
	stp		q6, q7, [sp, 0x060]
	stp		q8, q9, [sp, 0x080]
	stp		q10, q11, [sp, 0x0a0]
	stp		q12, q13, [sp, 0x0c0]
	stp		q14, q15, [sp, 0x0e0]
	stp		q16, q17, [sp, 0x100]
	stp		q18, q19, [sp, 0x120]
	stp		q20, q21, [sp, 0x140]
	stp		q22, q23, [sp, 0x160]
	stp		q24, q25, [sp, 0x180]
	stp		q26, q27, [sp, 0x1a0]
	stp		q28, q29, [sp, 0x1c0]
	stp		q30, q31, [sp, 0x1e0]
	bl		dv_catch_irq
	ldp		q30, q31, [sp, 0x1e0]
	ldp		q28, q29, [sp, 0x1c0]
	ldp		q26, q27, [sp, 0x1a0]
	ldp		q24, q25, [sp, 0x180]
	ldp		q22, q23, [sp, 0x160]
	ldp		q20, q21, [sp, 0x140]
	ldp		q18, q19, [sp, 0x120]
	ldp		q16, q17, [sp, 0x100]
	ldp		q14, q15, [sp, 0x0e0]
	ldp		q12, q13, [sp, 0x0c0]
	ldp		q10, q11, [sp, 0x0a0]
	ldp		q8, q9, [sp, 0x080]
	ldp		q6, q7, [sp, 0x060]
	ldp		q4, q5, [sp, 0x040]
	ldp		q2, q3, [sp, 0x020]
	ldp		q0, q1, [sp, 0x000]
	add		sp, sp, #0x200
	/* Fall through into dv_exception_return */

/* ==============================================
 * Exception return
 * This routine restores all that was saved in dv_exc_save_scratch.
 * Except thread SP and ESR ;-)
 * ==============================================
*/
dv_exception_return:
	ldp		x0, x1, [sp, #0xa0]
	msr		SPSR_EL1, x1				/* Saved PSTATE */
	msr		ELR_EL1, x0					/* PC where exception occurred */
	ldp		x18, x30, [sp, #0x90]
	ldp		x16, x17, [sp, #0x80]
	ldp		x14, x15, [sp, #0x70]
	ldp		x12, x13, [sp, #0x60]
	ldp		x10, x11, [sp, #0x50]
	ldp		x8, x9, [sp, #0x40]
	ldp		x6, x7, [sp, #0x30]
	ldp		x4, x5, [sp, #0x20]
	ldp		x2, x3, [sp, #0x10]
	ldp		x0, x1, [sp, #0x00]
	sub		sp, sp, #0xc0
	eret
