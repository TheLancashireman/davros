/*	dv-init-core.c - initialise an arm64 core
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
#include <kernel/h/dv-kconfig.h>
#include <dv-arm64-core.h>
#include <kernel/h/dv-stdio.h>

#ifndef DV_PRJ_VBAR_EL2
#define DV_PRJ_VBAR_EL2		((dv_u64_t)0)
#endif

#ifndef DV_PRJ_VBAR_EL3
#define DV_PRJ_VBAR_EL3		((dv_u64_t)0)
#endif

/* dv_init_core() - initialise the core
 *
 * Most of the register values are "safe" values. Some of them get overwritten later.
 * For davros, we're going to drop down to EL1 in the non-secure world. All we have to do
 * is avoid trying to initialise the EL2/EL3 registers if we're already at a lower level.
*/
void dv_init_core(void)
{
	dv_u32_t el = dv_get_el();

	if ( el >= 3 )
	{
		dv_kprintf("dv_arm64_msr(ELR_EL3, 0);\n");
		dv_arm64_msr(ELR_EL3, 0);
		dv_kprintf("dv_arm64_msr(VBAR_EL3, 0x%x%08x);\n", (dv_u32_t)(DV_PRJ_VBAR_EL3>>32), (dv_u32_t)DV_PRJ_VBAR_EL3);
		dv_arm64_msr(VBAR_EL3, DV_PRJ_VBAR_EL3);
		dv_kprintf("dv_arm64_msr(SCR_EL3, 0xc81);\n");
		dv_arm64_msr(SCR_EL3, 0xc81);						/* ToDo: explain "magic number" */
		dv_kprintf("dv_arm64_msr(SCTLR_EL3, 0);\n");
		dv_arm64_msr(SCTLR_EL3, 0);
		dv_kprintf("dv_arm64_msr(CPTR_EL3, 0);\n");
		dv_arm64_msr(CPTR_EL3, 0);
		dv_kprintf("dv_arm64_msr(MDCR_EL3, 0);\n");
		dv_arm64_msr(MDCR_EL3, 0);
	}

	if ( el >= 2 )
	{
		dv_kprintf("dv_arm64_msr(ELR_EL2, 0);\n");
		dv_arm64_msr(ELR_EL2, 0);
		dv_kprintf("dv_arm64_msr(VBAR_EL2, 0x%x%08x);\n", (dv_u32_t)(DV_PRJ_VBAR_EL2>>32), (dv_u32_t)DV_PRJ_VBAR_EL2);
		dv_arm64_msr(VBAR_EL2, DV_PRJ_VBAR_EL2);
		dv_kprintf("dv_arm64_msr(HCR_EL2, 0x80000000);\n");
		dv_arm64_msr(HCR_EL2, 0x80000000);					/* RW = 1 ==> execution state for EL1 is aarch64 */
		dv_kprintf("dv_arm64_msr(SCTLR_EL2, 0);\n");
		dv_arm64_msr(SCTLR_EL2, 0);
		dv_kprintf("dv_arm64_msr(VTTBR_EL2, 0);\n");
		dv_arm64_msr(VTTBR_EL2, 0);
		dv_kprintf("dv_arm64_msr(CPTR_EL2, 0);\n");
		dv_arm64_msr(CPTR_EL2, 0);
		dv_kprintf("dv_arm64_msr(MDCR_EL2, 0);\n");
		dv_arm64_msr(MDCR_EL2, 0);
		dv_kprintf("dv_arm64_msr(CNTVOFF_EL2, 0);\n");
		dv_arm64_msr(CNTVOFF_EL2, 0);
	}

	if ( el >= 1 )
	{
		dv_kprintf("dv_arm64_msr(ELR_EL1, 0);\n");
		dv_arm64_msr(ELR_EL1, 0);
		dv_kprintf("dv_arm64_msr(SPSR_EL1, 0);\n");
		dv_arm64_msr(SPSR_EL1, 0);
		dv_kprintf("dv_arm64_msr(SCTLR_EL1, 0);\n");
		dv_arm64_msr(SCTLR_EL1, 0);

		dv_kprintf("tlbi ALLE1\n");
		__asm__ volatile("tlbi ALLE1");

		dv_u64_t cpacr = dv_arm64_mrs(CPACR_EL1)|0x300000;		/* FPEN = b11 ==> disable FPU instruction traps */
		dv_kprintf("dv_arm64_msr(CPACR_EL1, 0x%08x);\n", (dv_u32_t)cpacr);
		dv_arm64_msr(CPACR_EL1, cpacr);

		dv_kprintf("dv_arm64_msr(S3_1_C15_C2_1, 0x40);  (S3_1_C15_C2_1 is CPUECTLR_EL1)\n");
		dv_arm64_msr(S3_1_C15_C2_1, 0x40);						/* ToDo: doc. reference? Is this an RCAR register? */
	}
	else
	{
		dv_kprintf("PANIC! Started at EL0\n");
	}
}
