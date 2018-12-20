/*	dv-reset.S - pi-zero startup code for davroska
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

/* dv_reset() - startup code after reset
 *
 * Reset vector jumps here.
 * Initialise processor then call dv_board_start.
*/
	.globl	dv_trap_reset
	.extern	dv_board_start
	.extern dv_kernstacktop
	.extern dv_catch_reset

	.text

dv_trap_reset:

/* Go to supervisor mode, disable IRQ and FIQ.
*/
	msr		cpsr_c, #(DV_ARM_MODE_SVC+DV_ARM_IRQ_DIS+DV_ARM_FIQ_DIS)

	ldr		r3, =dv_kernstacktop
	ldr		sp, [r3]
	ldr		lr, =dv_catch_reset
	mov		r0, #0
	b		dv_board_start
