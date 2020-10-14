/*	dv-saveregs.s - ARM save current CPU registers to structure
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

	.globl	dv_saveregs

	.text

/* dv_saveregs() - save all CPU registers to register store r
 *
 * Parameter r in r0
*/
dv_saveregs:
	stmia	r0, {r0-r12}								/* save r0-r12 in regs structure */

	mov		r1, lr										/* pc == link reg */
	mrs		r2, cpsr									/* current mode */
	mov		r3, sp										/* stack pointer */
	mov		r4, lr										/* link reg */
	add		r0, r0, #DV_OFFSET_regs_pc					/* r0 = &(r->sp) */
	stmia	r0, {r1-r4}

	bx		lr
