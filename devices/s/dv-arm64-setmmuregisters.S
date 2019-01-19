/*	dv-arm64-setmmuregisters.S - set all the MMU-related registers (TTBR, SCTLR, MAIR, TCR)
 *
 *	Copyright 2019 David Haworth
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

	.globl	dv_setMMUregisters

	.text

/* dv_setMMUregisters() - set TTBR0_EL1, MAIR_EL1, TCR_EL1 and SCTLR_EL1 from parameters
 *
 * Parameters:
 *	x0	- TTBR0_EL1
 *	x1	- MAIR_EL1
 *	x2	- TCR_EL1
 *	x3	- SCTLR_EL1
*/

dv_setMMUregisters:
	dsb     sy
	msr     TTBR0_EL1, x0
	msr     TTBR1_EL1, x0
	msr     TCR_EL1, x2
	msr     MAIR_EL1, x1
	msr     SCTLR_EL1, x3
	tlbi    VMALLE1
	dsb     sy
	isb     sy
	ret
