/*	dv-initmemorymanagement.c - initialise the ARM memory management systems
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
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include DV_H_MMU
#include <kernel/h/dv-stdio.h>

/* ToDo: all these attributes are read/write at the moment.
*/
#define L1_ATTR		(DV_V6MMUL1_L2B | DV_V6MMUL1_NS)
#define L2_ATTR_V	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1)
#define L2_ATTR_C	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1)
#define L2_ATTR_RO	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1|DV_V6MMUL2_XN)
#define L2_ATTR_RW	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1|DV_V6MMUL2_XN)
#define L2_ATTR_IO	(DV_V6MMUL2_PB|DV_V6MMUL2_AP_1|DV_V6MMUL2_XN)
        

extern dv_u32_t dv_start_text, dv_end_text;
extern dv_u32_t dv_start_rodata, dv_end_rodata; 
extern dv_u32_t dv_start_stack_c0, dv_end_stack_c0;
extern dv_u32_t dv_start_data_c0, dv_end_bss_c0;


/* dv_init_memory_management() - initialise the memory management systems
*/
void dv_init_memory_management(dv_kernel_t *kvars)
{
	dv_u32_t p;

	dv_kprintf("dv_init_memory_management()\n");

	dv_init_pagetable(kvars);

	/* Map a page for the exception vectors.
	*/
	dv_mmu_map_page(kvars, (void *)DV_VECTOR_LOCATION, (void *)DV_VECTOR_LOCATION, L1_ATTR, L2_ATTR_V);

	/* Map code pages for the text region.
	*/
	for ( p = (dv_u32_t)&dv_start_text; p < (dv_u32_t)&dv_end_text; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, L1_ATTR, L2_ATTR_C);
	}

	/* Map read-only pages (no execute) for the rodata region.
	*/
	for ( p = (dv_u32_t)&dv_start_rodata; p < (dv_u32_t)&dv_end_rodata; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, L1_ATTR, L2_ATTR_RO);
	}

	/* Map read/write pages for the stack region.
	*/
	for ( p = (dv_u32_t)&dv_start_stack_c0; p < (dv_u32_t)&dv_end_stack_c0; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, L1_ATTR, L2_ATTR_RW);
	}

	/* Map read/write pages for the data/bss region.
	*/
	for ( p = (dv_u32_t)&dv_start_data_c0; p < (dv_u32_t)&dv_end_bss_c0; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, L1_ATTR, L2_ATTR_RW);
	}

	/* ToDo: map pages for the page tables.
	*/
	/* ToDo: map pages for the I/O regions (int. controller, UART, timer, GPIO etc.)
	*/
}
