/*	dv-armv6-mmu.c - arm v6 MMU setup for davroska 
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
#define DV_ASM	0
#include <dv-config.h>
#include <dv-devices.h>
#include <dv-armv6-mmu.h>
#include <dv-arm-cp15.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif

/* davroska doesn't support memory protection or virtualized addresses.
 * So the MMU setup is fixed at startup
 *
 * It uses a single L1 page table of 4096 entries. Each entry is a 1 MiB section.
 * Addresses from 0 to dv_end_mem are mapped as memory (cacheable)
 * Addresses from dv_peripheral to dv_peripheral_end are mapped as peripherals (not cacheable)
 * All other addresses (including video RAM) are mapped as inaccessible
 *
 * The pagetable must start on a 16 KiB boundary and is 16 KiB long. The pagetable is reserved
 * in the linker script.
*/
extern struct dv_armv6_l1pagetable_s dv_pagetable;
extern dv_u32_t dv_end_mem;
extern dv_u32_t dv_peripheral;
extern dv_u32_t dv_peripheral_end;

void dv_armv6_mmu_setup(void)
{
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() - initialize page table\n");
#endif
	for ( int i = 0; i < DV_ARMV6MMU_L1_SIZE; i++ )
		dv_pagetable.l1page[i] = 0;

	const dv_u32_t MiB = 1024*1024;
	const dv_u32_t addr_end_mem = (dv_u32_t)&dv_end_mem;
	const dv_u32_t addr_start_per = (dv_u32_t)&dv_peripheral;
	const dv_u32_t addr_end_per = (dv_u32_t)&dv_peripheral_end;

	dv_u32_t page = 0;

	/* Map the memory pages:
	 *	
	*/
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() - map the memory sections\n");
#endif
	for ( dv_u32_t addr = 0; addr < addr_end_mem; addr += MiB )
	{
#if DV_DEBUG
		dv_printf("dv_armv6_mmu_setup() - memory page %d at 0x%08x\n", page, addr);
#endif
		dv_pagetable.l1page[page] = DV_V6MMUL1_SEC |			/* 1 MiB section */
									DV_V6MMUL1_S_AP_UNO | 		/* Priv. rwx, user no access (APX=0, XN=0) */
									DV_V6MMUL1_ATTR_MEMORY |	/* Normal memory (TEX_1, B, C, S=0) */
																/* Global (nG=0), Domain 0 (DOM=0) */
									addr;						/* Physical address */
		page++;
	}

	/* Skip to first peripheral page
	*/
	page = addr_start_per >> 20;

	/* Map the peripheral pages
	*/
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() - map the peripheral sections - page = %d\n", page);
#endif
	for ( dv_u32_t addr = addr_start_per; addr < addr_end_per; addr += MiB )
	{
#if DV_DEBUG
		dv_printf("dv_armv6_mmu_setup() - peripheral page %d at 0x%08x\n", page, addr);
#endif
		dv_pagetable.l1page[page] = DV_V6MMUL1_SEC |			/* 1 MiB section */
									DV_V6MMUL1_S_AP_UNO | 		/* Priv. rwx, user no access (APX=0, XN=0) */
									DV_V6MMUL1_ATTR_SH_DEV |	/* Shared device (TEX_0, B, C=0, S=0) */
																/* Global (nG=0), Domain 0 (DOM=0) */
									addr;						/* Physical address */
		page++;
	}

	/* Caches shouldn't be enabled yet, so no need to flush/invalidate here
	*/
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() -  set TTBR0\n");
#endif
	dv_set_translation_table_base_0((dv_u32_t)&dv_pagetable | DV_TTB_C | DV_TTB_RGN_WB);
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() -  set domain access control\n");
#endif
	dv_set_domain_access_control(0x55555555);       /* Everything to "client" */
#if DV_DEBUG
	dv_printf("dv_armv6_mmu_setup() -  enable MMU\n");
#endif
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_M | DV_CP15_CTRL_XP);
}
