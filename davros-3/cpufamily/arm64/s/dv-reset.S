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
 *
 * On a raspberry pi 3 use
 *	kernel_old=1
 * in config.txt to load the image at 0. Also use
 *	arm_control=0x200				(start in ARM64 mode)
 *	disable_commandline_tags=1		(not sure, but davros-related images don't have command lines)
 * There might be more modern versions of these config options.
 * 
 * Initialise processor then call dv_board_start on core 0.
 * Cores 1, 2 and 3 loop until told what to do.
*/
	.globl	dv_reset
	.extern	dv_board_start

	.section	reset, "ax"

/* Jump past the area reserved for a startup stack.
*/
dv_reset:
	b		skip

/* Reserve 0x8000 bytes for the startup stacks; 0x2000 per core
 * The "-0x0004" is for the "b skip" instruction above.
*/
	.space 0x8000-0x0004,0

skip:
/*	Calculate the stack pointers for the four cores.
 *	The initial value of the SP (on cores 1..3) is the release address.
 *	Core 0 runs dv_board_start().
 *	dv_board_start() gets four parameters: its own SP and the release addresses of the other three cores.
 *
 *	The release mechanism doesn't seem to work when the release addresses are ***fc or ***f8 - why?
*/
	mov		sp, #0x8000
	ldr		x4, dv_reset_addr
	add		sp, sp, x4
	mov		x0, sp
	mov		x1, #0x6000
	add		x1, x1, x4
#if 0
	sub		x1, x1, #8
#endif
	mov		x2, #0x4000
	add		x2, x2, x4
#if 0
	sub		x2, x2, #8
#endif
	mov		x3, #0x2000
	add		x3, x3, x4
#if 0
	sub		x3, x3, #8
#endif

/*	Now find out which core we're on.
*/
	mrs		x5, mpidr_el1
	mov		x6, #0xFF000000
	bic		x5, x5, x6
	cbz		x5, core_zero
	sub		x6, x5, #1
	cbz		x6, core_one
	sub		x6, x5, #2
	cbz		x6, core_two
	sub		x6, x5, #3
	cbz		x6, core_three

	mrs		x0, mpidr_el1
	b		skip

dv_reset_addr:
	.quad	dv_reset

/* Core 0 - go to dv_board_start
*/
core_zero:
	bl		dv_board_start
	b		skip				/* if the board start function returns, rinse and repeat. */

/*	Cores 1, 2, 3 - spin until needed.
*/
core_one:
	mov		sp, x1
	b		core_common

core_two:
	mov		sp, x2
	b		core_common

core_three:
	mov		sp, x3

/* Cores 1, 2 and 3 wait here until a release address appears at the top of their stack.
*/
core_common:
	mov		x1, #0
	str		w1, [sp]
	mov		x0, #0

core_loop:
	ldr		w0, [sp]
	cbz		w0, core_loop
	bl		trampoline
	b		skip				/* if the board start function returns, rinse and repeat. */

trampoline:
	br		x0
