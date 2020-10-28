/*	dv-switch-el3el1.S - arm64 "switch from el3 to el1" function
 *
 *	Copyright David Haworth
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

	.globl	dv_switch_el3el1
	.text

/* dv_switch_el3el1() - drop from EL3 to EL1
 *
 * Returns to caller, but in EL1. For use when secure EL1 is needed (no EL2 in that case).
 *
 * See DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf page 51
*/

dv_switch_el3el1:
	mov		x0, sp				/* Copy current sp to EL2 and EL1 sp */
	msr		sp_el2, x0
	msr		sp_el1, x0
	msr		elr_el3, x30		/* Place function return address (lr) into exception return address */
	mov		x0, #5				/* PSTATE = EL3 */
	msr		spsr_el3, x0
	eret						/* Return to caller of function, but switch EL */
