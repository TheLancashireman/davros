/*	dv-armv6-mmu.c - mmu management functions for armv6 MMU
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-mempage.h>
#include <cpufamily/arm/h/dv-armv6-mmu.h>
#include <kernel/h/dv-stdio.h>

DV_COVDEF(armv6_mmu);

static inline void dv_armv6_mmu_new_l1_map(dv_kernel_t *kvars, dv_u32_t l1_ind, dv_u32_t l1_attr);

dv_armv6_l1pagetable_t dv_c0_l1_pagetable __attribute__((section("dv_pagetable")));

/* dv_armv6_mmu_init_pagetable() - initialise the MMU
 *
 * Initialises the page tables to map the memory at its physical addresses for the kernel.
*/
void dv_armv6_mmu_init_pagetable(dv_kernel_t *kvars)
{
	dv_kprintf("dv_armv6_mmu_init()\n");
	if ( kvars->cpu.page_table != DV_NULL )
	{
		dv_panic(dv_panic_initialisationerror, "dv_armv6_mmu_init", "page table pointer is not null");
	}

	/* Set the core's L1 page table.
	*/
	kvars->cpu.page_table = &dv_c0_l1_pagetable;
}

/* dv_armv6_mmu_map_page() - map a page to a virtual address
 *
 * Maps a physical page to a virtual address.
*/
void dv_armv6_mmu_map_page(dv_kernel_t *kvars, void *phys, void *virt, dv_u32_t l1_attr, dv_u32_t l2_attr)
{
	dv_kprintf("dv_armv6_mmu_map_page() mapping 0x%08x to physical address 0x%08x\n", virt, phys);
	dv_u32_t v_addr = (dv_u32_t)virt;
	dv_u32_t l1_ind = dv_armv6_virt_to_l1(v_addr);

	if ( kvars->cpu.page_table->l1page[l1_ind] == 0 )
	{
		dv_armv6_mmu_new_l1_map(kvars, l1_ind, l1_attr);
	}

	dv_u32_t l2_ent = kvars->cpu.page_table->l1page[l1_ind];
	dv_armv6_l2pagetable_t *l2_tbl = (dv_armv6_l2pagetable_t *)(l2_ent & DV_V6MMUL1_L2B_ADDR);
	dv_u32_t l2_ind = dv_armv6_virt_to_l2(v_addr);
	dv_u32_t p_addr = (dv_u32_t)phys & DV_V6MMUL2_ADDR;

	if ( l2_tbl->l2page[l2_ind] == 0 )
	{
		/* New mapping: put physical address and l2 attributes into l2 table.
		*/
		dv_kprintf("dv_armv6_mmu_map_page() l1_ind %d, l2_ind %d, value = 0x%08x\n", l1_ind, l2_ind, p_addr | l2_attr);
		l2_tbl->l2page[l2_ind] = p_addr | l2_attr;
	}
	else
	{
		dv_kprintf("dv_armv6_mmu_map_page() l1_ind %d, l2_ind %d, reusing existing l2 mapping 0x%08x\n",
						l1_ind, l2_ind, l2_tbl->l2page[l2_ind]);
		if ( l2_tbl->l2page[l2_ind] != (p_addr | l2_attr) )
		{
			dv_panic(dv_panic_initialisationerror, "dv_armv6_mmu_map_page", "virtual page already mapped differently");
		}
	}
}

/* dv_armv6_mmu_new_l1_map() - create a new l1 mapping
 *
 * Allocates a new l2 table and maps it to an l1 entry.
 *
 * Space for the tables is allocated from the free page pool. Each page of memory (4KiB) can
 * hold 4 l2 tables (256*4 bytes = 1KiB each).
 * The in-use counter of the page allocator keeps track of the number of tables in use in each page.
*/

static inline void dv_armv6_mmu_new_l1_map(dv_kernel_t *kvars, dv_u32_t l1_ind, dv_u32_t l1_attr)
{
	dv_kprintf("dv_armv6_mmu_new_l1_map() l1_ind %d, l1_attr 0x%08x\n", l1_ind, l1_attr);
	if ( kvars->cpu.l2_table_page == DV_NULL )
	{
		kvars->cpu.l2_table_page = dv_allocate_page(kvars);
		dv_kprintf("dv_armv6_mmu_new_l1_map(): new l2 array block at 0x%08x\n", kvars->cpu.l2_table_page->page);
	}
	else
	{
		/* Each allocated page holds up to 4 L2 tables.
		*/
		dv_kprintf("dv_armv6_mmu_new_l1_map(): existing l2 array block at 0x%08x, index %d\n",
			kvars->cpu.l2_table_page->page, kvars->cpu.l2_table_page->n_use);
		kvars->cpu.l2_table_page->n_use++;
		if ( kvars->cpu.l2_table_page->n_use > 4 )
		{
			dv_panic(dv_panic_initialisationerror, "dv_armv6_mmunew_l1_map", "page use count exceeded");
		}
	}

	dv_armv6_l2pagetable_t *l2_tbl = (dv_armv6_l2pagetable_t *)kvars->cpu.l2_table_page->page;
	dv_u32_t l1_ent = (dv_u32_t)&l2_tbl[kvars->cpu.l2_table_page->n_use-1];
	kvars->cpu.page_table->l1page[l1_ind] = l1_ent | l1_attr;

	if ( kvars->cpu.l2_table_page->n_use >= 4 )
	{
		/* All four L2 pages in this block are used.
		*/
		dv_kprintf("dv_armv6_mmu_new_l1_map(): l2 array block full\n");
		kvars->cpu.l2_table_page = DV_NULL;
	}
}
