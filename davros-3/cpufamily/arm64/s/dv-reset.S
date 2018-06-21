/*	dv-reset.S - ARM64 startup code for davros
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

#ifndef DV_N_CORES
#error "DV_N_CORES is nto defined"
#endif

/* dv_reset() - startup code after reset
 *
 * This code is placed at the reset location, wherever that happens to be.
 * Initialise processor then call dv_board_start on core 0.
 * Cores 1, 2 and 3 loop until told what to do.
*/
	.globl	dv_reset
	.extern	dv_board_start

	.section	.reset

/* Jump past the area reserved for a startup stack.
*/
dv_reset:
	b		skip

/* Reserve 0x8000 bytes for the startup stacks; 0x2000 per core
 * The "-0x0004" is for the "b skip" instruction above.
*/
.space 0x8000-0x0004,0

skip:
	mrs		x0, mpidr_el1
	mov		x1, #0xFF000000
	bic		x0, x0, x1
	cbz		x0, core_zero
	sub		x1, x0, #1
	cbz		x1, core_one
	sub		x1, x0, #2
	cbz		x1, core_two
	sub		x1, x0, #3
	cbz		x1, core_three

	mrs		x0, mpidr_el1
	b		hang

/* Core 0 - go to dv_board_start
*/
core_zero:
	mov		sp, #0x8000
	mov		x1, #dv_reset 
	add		sp, sp, x1
	bl		dv_board_start

/* If dv_board_start (or the release function) returns, wait here.
*/
hang:
	b		hang

core_one:
	mov		sp, #0x6000
	b		core_common

core_two:
	mov		sp, #0x4000
	b		core_common

core_three:
	mov		sp, #0x2000

/* Cores 1, 2 and 3 wait here until a release address appears at the top of their stack.
*/
core_common:
	mov		x1, #dv_reset
	add		sp, sp, x1
	sub		sp, sp, #4
	mov		x1, #0
	str		w1, [sp]
	mov		x0, #0

core_loop:
	ldr		w0,[sp]
	cbz		w0,core_loop
	bl		trampoline
	b		hang

trampoline:
	br		x0
