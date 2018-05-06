/*	dv-arm64-returntothread.s - arm64 thread resumption.
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
#include <kernel/h/dv-thread.h>
#include <cpufamily/arm64/h/dv-arm64-registers.h>

	.globl	dv_arm64_return_to_thread

	.text

/* dv_arm64_return_to_thread(dv_registers_t *r, dv_stackword_t *ksp, dv_u32_t pt) - return to the thread
 *
 * Parameter r in x0  (register store)
 *			 ksp in x2 (kernel stack pointer)
 *			 pt in x1 (page table)
*/
dv_arm64_return_to_thread:

	mov		sp, x1										/* Set the kernel stack pointer */

#if 0		/* Switch page tables? */
	msr		TTBR0_EL1, x2								/* No need for isb because eret synchronizes */
#endif

	/* We only read kernel data after this, so we can do it after the page table switch.
	*/
	ldp		x2, x3, [x0, #DV_OFFSET_regs_psr]			/* Thread's SP and PSTATE */
	msr		SP_EL0, x3
	msr		SPSR_EL1, x2

	ldp		x30, x3, [x0, #DV_OFFSET_regs_gpr0+0xf0]	/* Restore thread's PC and x30 */
	msr		ELR_EL1, x3

	ldp		x28, x29, [x0, #DV_OFFSET_regs_gpr0+0xe0]	/* Restore rest of working registers */
	ldp		x26, x27, [x0, #DV_OFFSET_regs_gpr0+0xd0]
	ldp		x24, x25, [x0, #DV_OFFSET_regs_gpr0+0xc0]
	ldp		x22, x23, [x0, #DV_OFFSET_regs_gpr0+0xb0]
	ldp		x20, x21, [x0, #DV_OFFSET_regs_gpr0+0xa0]
	ldp		x18, x19, [x0, #DV_OFFSET_regs_gpr0+0x90]
	ldp		x16, x17, [x0, #DV_OFFSET_regs_gpr0+0x80]
	ldp		x14, x15, [x0, #DV_OFFSET_regs_gpr0+0x70]
	ldp		x12, x13, [x0, #DV_OFFSET_regs_gpr0+0x60]
	ldp		x10, x11, [x0, #DV_OFFSET_regs_gpr0+0x50]
	ldp		x8, x9, [x0, #DV_OFFSET_regs_gpr0+0x40]
	ldp		x6, x7, [x0, #DV_OFFSET_regs_gpr0+0x30]
	ldp		x4, x5, [x0, #DV_OFFSET_regs_gpr0+0x20]
	ldp		x2, x3, [x0, #DV_OFFSET_regs_gpr0+0x10]
	ldp		x0, x1, [x0, #DV_OFFSET_regs_gpr0+0x00]		/* Must be last because it trashes the regs pointer */

	eret
