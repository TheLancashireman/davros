/*	dv-switch-el3el2.S - arm64 "switch from el3 to el2" function
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
	.globl	dv_switch_el3el2
	.text

/* dv_switch_el3el2() - drop from EL3 to EL2
 *
 * Returns to caller, but in EL2
 *
 * See DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf page 51
*/

dv_switch_el3el2:
	mov		x0, sp				/* Copy current sp to EL2 sp */
	msr		sp_el2, x0
	msr		elr_el3, x30		/* Place function return address (lr) into exception return address */
	mov		x0, #9				/* PSTATE = EL2 */
	msr		spsr_el3, x0
	eret						/* Return to caller of function, but switch EL */
