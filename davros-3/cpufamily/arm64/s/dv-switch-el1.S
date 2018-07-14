/*	dv-switch-el1.S - arm64 "switch to el1" function
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

	.globl	dv_switch_el1
	.text

/* dv_switch_el1() - drop from EL2 to EL1
 *
 * Returns to caller, but in EL1
 *
 * Parameters:
 *	x0 - desired program state (SPSR_EL2 restored to PSR)
 *
 * Preconditions
 * See DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf page 51
*/

dv_switch_el1:
	msr		sctlr_el1, xzr		/* Set SCTLR_EL1 to safe state */
	msr		spsr_el2, x0		/* Place selected program state into SPSR */

	mrs		x0, hcr_el2
	orr		x0, x0, #0x80000000	/* Set RW=1: execution state of lower level is aarch64 */
	msr		hcr_el2, x0

	msr		elr_el2, x30		/* Place function return address (lr) into exception return address */
	mov		x0, sp				/* Copy EL2 SP to EL1 SP */
	msr		sp_el1, x0
	eret						/* Return to caller of function, but switch EL */
