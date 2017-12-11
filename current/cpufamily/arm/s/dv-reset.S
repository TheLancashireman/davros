/*	dv-reset.S - ARM startup code for davros
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
#include <cpufamily/arm/h/dv-arm-registers.h>

#ifndef DV_N_CORES
#error "DV_N_CORES is nto defined"
#endif

/* dv_reset() - startup code after reset
 *
 * Reset vector jumps here.
 * Initialise processor then call dv_board_start.
*/
	.globl	dv_trap_reset
	.extern	dv_board_start
	.extern dv_c0_kernstacktop

	.text

dv_trap_reset:

/* Go to supervisor mode, disable IRQ and FIQ.
*/
	msr		cpsr_c, #(DV_ARM_MODE_SVC+DV_ARM_IRQ_DIS+DV_ARM_FIQ_DIS)

#if DV_N_CORES <= 1					/* Single-core */
	b		core0_start
#endif

#if DV_N_CORES >= 2
	mrc		p15,0,r5,c0,c0,5		/* Multiprocessor affinity register */
	and		r0, r5, #3				/* Discard all but cpu ID */
	cmp		r0, #0
	beq		core0_start

	cmp		r0, #1
	beq		core1_start
#endif
#if DV_N_CORES >= 3
	cmp		r0, #2
	beq		core2_start
#endif
#if DV_N_CORES >= 4
	cmp		r0, #3
	beq		core4_start
#endif

/* Unknown cores loop here. Also loop here if dv_board_start returns
*/
stop:
	wfi
	b		stop

core0_start:
/* Set the supervisor (kernel) stack pointer
 * Then call dv_board_start(0)
*/
	ldr		r3, =dv_c0_kernstacktop
	ldr		sp, [r3]
	ldr		lr, =stop
	mov		r0, #0
	b		dv_board_start
