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

	.globl	dv_vectortable

	.globl		dv_exc_from_priv
	.globl		dv_exc_priv_sync
	.globl		dv_exc_priv_irq
	.globl		dv_exc_priv_fiq
	.globl		dv_exc_priv_serror

	.globl		dv_exc_from_kernel
	.globl		dv_exc_kernel_sync
	.globl		dv_exc_kernel_irq
	.globl		dv_exc_kernel_fiq
	.globl		dv_exc_kernel_serror

	.globl		dv_exc_from_nonpriv
	.globl		dv_exc_nonpriv_sync
	.globl		dv_exc_nonpriv_irq
	.globl		dv_exc_nonpriv_fiq
	.globl		dv_exc_nonpriv_serror

	.globl		dv_exc_from_aa32
	.globl		dv_exc_aa32_sync
	.globl		dv_exc_aa32_irq
	.globl		dv_exc_aa32_fiq
	.globl		dv_exc_aa32_serror

	.extern		dv_fiq_from_aarch32
	.extern		dv_fiq_from_kernel
	.extern		dv_fiq_from_thread
	.extern		dv_irq_from_aarch32
	.extern		dv_irq_from_kernel
	.extern		dv_irq_from_thread
	.extern		dv_syncexc_from_kernel
	.extern		dv_syncexc_from_thread
	.extern		dv_syncexc_from_aarch32
	.extern		dv_syserror_from_aarch32
	.extern		dv_syserror_from_kernel
	.extern		dv_syserror_from_thread

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

/* A macro to save the registers to the current thread's register store.
 *
 * This macro is used when the exception comes from a thread (SP0).
 * If we use it when the exception comes from the kernel, we trash the thread's registers.
 * That's OK for starters :-)
 *
 * This macro must not be more than 31 instructions.
*/
	.macro	dv_exc_save_all
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
	stp		x0, x1, [x30, #DV_OFFSET_regs_gpr0+0x000]
	stp		x2, x3, [x30, #DV_OFFSET_regs_gpr0+0x010]
	stp		x4, x5, [x30, #DV_OFFSET_regs_gpr0+0x020]
	stp		x6, x7, [x30, #DV_OFFSET_regs_gpr0+0x030]
	stp		x8, x9, [x30, #DV_OFFSET_regs_gpr0+0x040]
	stp		x10, x11, [x30, #DV_OFFSET_regs_gpr0+0x050]
	stp		x12, x13, [x30, #DV_OFFSET_regs_gpr0+0x060]
	stp		x14, x15, [x30, #DV_OFFSET_regs_gpr0+0x070]
	stp		x16, x17, [x30, #DV_OFFSET_regs_gpr0+0x080]
	stp		x18, x19, [x30, #DV_OFFSET_regs_gpr0+0x090]
	stp		x20, x21, [x30, #DV_OFFSET_regs_gpr0+0x0a0]
	stp		x22, x23, [x30, #DV_OFFSET_regs_gpr0+0x0b0]
	stp		x24, x25, [x30, #DV_OFFSET_regs_gpr0+0x0c0]
	stp		x26, x27, [x30, #DV_OFFSET_regs_gpr0+0x0d0]
	stp		x28, x29, [x30, #DV_OFFSET_regs_gpr0+0x0e0]
	mrs		x0, TPIDR_EL0									/* Recover original x30 */
	mrs		x1, ELR_EL1										/* PC where exception occurred */
	mrs		x2, SPSR_EL1									/* Saved PSTATE */
	mrs		x3, SP_EL0										/* Thread SP */
	stp		x0, x1, [x30, #DV_OFFSET_regs_gpr0+0x0f0]
	stp		x2, x3, [x30, #DV_OFFSET_regs_psr]
	.endm

/* Now the EL1 vector table
*/
	.text

/* ============================================
 * Exceptions from privileged thread (EL1, SP0)
 * ============================================
*/
	.align	7
dv_vectortable:
dv_exc_from_priv:
dv_exc_priv_sync:				/* Synchronous exception from privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syncexc_from_thread

	.align	7
dv_exc_priv_irq:				/* Irq from privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_irq_from_thread

	.align	7
dv_exc_priv_fiq:				/* Fiq from privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_fiq_from_thread

	.align	7
dv_exc_priv_serror:				/* SError from privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syserror_from_thread

/* =================================
 * Exceptions from kernel (EL1, SP1)
 * =================================
*/
	.align	7
dv_exc_from_kernel:
dv_exc_kernel_sync:				/* Synchronous exception from kernel */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syncexc_from_kernel

	.align	7
dv_exc_kernel_irq:				/* Irq exception from kernel */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_irq_from_kernel

	.align	7
dv_exc_kernel_fiq:				/* Fiq exception from kernel */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_fiq_from_kernel

	.align	7
dv_exc_kernel_serror:			/* SError from kernel */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syserror_from_kernel

/* ================================================
 * Exceptions from non-privileged thread (EL0, SP0)
 * ================================================
*/
	.align	7
dv_exc_from_nonpriv:
dv_exc_nonpriv_sync:			/* Synchronous exception from non-privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syncexc_from_thread

	.align	7
dv_exc_nonpriv_irq:				/* Irq from non-privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_irq_from_thread

	.align	7
dv_exc_nonpriv_fiq:				/* Fiq from non-privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_fiq_from_thread

	.align	7
dv_exc_nonpriv_serror:			/* SError from non-privileged thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syserror_from_thread

/* ==============================================
 * Exceptions from aarch32 thread (not supported)
 * ==============================================
*/
	.align	7
dv_exc_from_aa32:
dv_exc_aa32_sync:				/* Synchronous exception from aarch32 thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syncexc_from_aarch32

	.align	7
dv_exc_aa32_irq:				/* Irq from aarch32 thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_irq_from_aarch32

	.align	7
dv_exc_aa32_fiq:				/* Fiq from aarch32 thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_fiq_from_aarch32

	.align	7
dv_exc_aa32_serror:				/* SError from aarch32 thread */
	dv_exc_save_all				/* Save all registers to thread (macro) */
	b		dv_syserror_from_aarch32

	.align	7
dv_vectortable_end:
