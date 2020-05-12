/*	dv-vectors.s - ARM64 exception and interrupt vectors
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
#include <kernel/h/dv-asm.h>
#include <kernel/h/dv-kernel.h>
#include <cpufamily/arm64/h/dv-arm64-registers.h>

	.globl		dv_vectortable

	.extern		dv_catch_aarch32_fiq
	.extern		dv_catch_kernel_fiq
	.extern		dv_catch_thread_fiq

	.extern		dv_catch_aarch32_irq
	.extern		dv_catch_kernel_irq
	.extern		dv_catch_thread_irq

	.extern		dv_catch_aarch32_synchronous_exception
	.extern		dv_catch_kernel_synchronous_exception
	.extern		dv_catch_thread_synchronous_exception

	.extern		dv_catch_aarch32_syserror
	.extern		dv_catch_kernel_syserror
	.extern		dv_catch_thread_syserror

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

/* A macro to save most of the registers to the current thread's register store.
 *
 * This macro is used when the exception comes from a thread (SP0).
 * If we use it when the exception comes from the kernel, we trash the thread's registers.
 * That's OK for starters :-)
 *
 * This macro must not be more than 28 instructions.
*/
	.macro	dv_exc_save_most
	msr		TPIDR_EL0, x30		/* Free up a register */

#if 0		/* Switch page tables? */
	dsb		SY					/* Barrier: ensure no writes are pending */
	mrs		x30, TTBR1_EL1		/* Load kernel page table address */
	msr		TTBR0_EL1, x30		/* Set current page table */
	isb		SY					/* Barrier: ensure new page table is effective */
#endif

	mrs		x30, TPIDRRO_EL0								/* kvars */
	ldr		x30, [x30, #DV_OFFSET_kvars_current_thread]		/* current_thread */
	ldr		x30, [x30, #DV_OFFSET_thr_regs]					/* registers */
	stp		x0, x1, [x30, #DV_OFFSET_regs_gpr0+0x00]
	stp		x2, x3, [x30, #DV_OFFSET_regs_gpr0+0x10]
	stp		x4, x5, [x30, #DV_OFFSET_regs_gpr0+0x20]
	stp		x6, x7, [x30, #DV_OFFSET_regs_gpr0+0x30]
	stp		x8, x9, [x30, #DV_OFFSET_regs_gpr0+0x40]
	stp		x10, x11, [x30, #DV_OFFSET_regs_gpr0+0x50]
	stp		x12, x13, [x30, #DV_OFFSET_regs_gpr0+0x60]
	stp		x14, x15, [x30, #DV_OFFSET_regs_gpr0+0x70]
	stp		x16, x17, [x30, #DV_OFFSET_regs_gpr0+0x80]
	stp		x18, x19, [x30, #DV_OFFSET_regs_gpr0+0x90]
	stp		x20, x21, [x30, #DV_OFFSET_regs_gpr0+0xa0]
	stp		x22, x23, [x30, #DV_OFFSET_regs_gpr0+0xb0]
	stp		x24, x25, [x30, #DV_OFFSET_regs_gpr0+0xc0]
	stp		x26, x27, [x30, #DV_OFFSET_regs_gpr0+0xd0]
	stp		x28, x29, [x30, #DV_OFFSET_regs_gpr0+0xe0]
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
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syncexc_from_thread
	b		dv_exc_common

	.balign	8
_dv_syncexc_from_thread:
	.quad	dv_catch_thread_synchronous_exception

	.balign	128
dv_exc_priv_irq:				/* Irq from privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_irq_from_thread
	b		dv_exc_common

	.balign	8
_dv_irq_from_thread:
	.quad	dv_catch_thread_irq

	.balign	128
dv_exc_priv_fiq:				/* Fiq from privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_fiq_from_thread
	b		dv_exc_common

	.balign	8
_dv_fiq_from_thread:
	.quad	dv_catch_thread_fiq

	.balign	128
dv_exc_priv_serror:				/* SError from privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syserror_from_thread
	b		dv_exc_common

	.balign	8
_dv_syserror_from_thread:
	.quad	dv_catch_thread_syserror

/* =================================
 * Exceptions from kernel (EL1, SP1)
 * =================================
*/
	.balign	128
dv_exc_kernel_sync:				/* Synchronous exception from kernel */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syncexc_from_kernel
	b		dv_exc_common

	.balign	8
_dv_syncexc_from_kernel:
	.quad	dv_catch_kernel_synchronous_exception

	.balign	128
dv_exc_kernel_irq:				/* Irq exception from kernel */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_irq_from_kernel
	b		dv_exc_common

	.balign	8
_dv_irq_from_kernel:
	.quad	dv_catch_kernel_irq

	.balign	128
dv_exc_kernel_fiq:				/* Fiq exception from kernel */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_fiq_from_kernel
	b		dv_exc_common

	.balign	8
_dv_fiq_from_kernel:
	.quad	dv_catch_kernel_fiq

	.balign	128
dv_exc_kernel_serror:			/* SError from kernel */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syserror_from_kernel
	b		dv_exc_common

	.balign	8
_dv_syserror_from_kernel:
	.quad	dv_catch_kernel_syserror

/* ================================================
 * Exceptions from non-privileged thread (EL0, SP0)
 * ================================================
*/
	.balign	128
dv_exc_nonpriv_sync:			/* Synchronous exception from non-privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syncexc_from_thread
	b		dv_exc_common

	.balign	8

	.balign	128
dv_exc_nonpriv_irq:				/* Irq from non-privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_irq_from_thread
	b		dv_exc_common

	.balign	8

	.balign	128
dv_exc_nonpriv_fiq:				/* Fiq from non-privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_fiq_from_thread
	b		dv_exc_common

	.balign	8

	.balign	128
dv_exc_nonpriv_serror:			/* SError from non-privileged thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syserror_from_thread
	b		dv_exc_common

	.balign	8

/* ==============================================
 * Exceptions from aarch32 thread (not supported)
 * ==============================================
*/
	.balign	128
dv_exc_aa32_sync:				/* Synchronous exception from aarch32 thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syncexc_from_aarch32
	b		dv_exc_common

	.balign	8
_dv_syncexc_from_aarch32:
	.quad	dv_catch_aarch32_synchronous_exception

	.balign	128
dv_exc_aa32_irq:				/* Irq from aarch32 thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_irq_from_aarch32
	b		dv_exc_common

	.balign	8
_dv_irq_from_aarch32:
	.quad	dv_catch_aarch32_irq

	.balign	128
dv_exc_aa32_fiq:				/* Fiq from aarch32 thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_fiq_from_aarch32
	b		dv_exc_common

	.balign	8
_dv_fiq_from_aarch32:
	.quad	dv_catch_aarch32_fiq

	.balign	128
dv_exc_aa32_serror:				/* SError from aarch32 thread */
	dv_exc_save_most			/* Save registers to thread (macro) */
    ldr     x1, _dv_syserror_from_aarch32
	b		dv_exc_common

	.balign	8
_dv_syserror_from_aarch32:
	.quad	dv_catch_aarch32_syserror

/* ==============================================
 * Common exception handler.
 * ==============================================
 *
 * Assumes x30 is the address of a valid register store.
 * Assumes x1 contains the address of the handler function to jump to.
 * Passes kvars to the handler function in x0
*/
dv_exc_common:
	mrs		x2, TPIDR_EL0									/* Recover original x30 */
	mrs		x3, ELR_EL1										/* PC where exception occurred */
	stp		x2, x3, [x30, #DV_OFFSET_regs_gpr0+0xf0]
	mrs		x4, SPSR_EL1									/* Saved PSTATE */
	mrs		x5, SP_EL0										/* Thread SP */
	stp		x4, x5, [x30, #DV_OFFSET_regs_psr]
	mrs		x6, ESR_EL1										/* Exception syndrome */
	str		w6, [x30, #DV_OFFSET_regs_esr]
	mrs		x0, TPIDRRO_EL0									/* kvars */
	ldr		x3, [x0, #DV_OFFSET_kvars_kernelsp]				/* kvars->kernelsp */
	mov		sp, x3
	br		x1
