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
#include <devices/h/dv-arm-bcm2835-aux.h>
#include <devices/h/dv-arm-bcm2835-gpio.h>
#include <devices/h/dv-arm-bcm2835-interruptcontroller.h>
#include <devices/h/dv-arm-bcm2835-systimer.h>
#include <cpufamily/arm/h/dv-arm-cache.h>
#include <cpufamily/arm/h/dv-arm-cp15.h>
#include <kernel/h/dv-stdio.h>

extern dv_u32_t dv_start_text, dv_end_text;
extern dv_u32_t dv_start_rodata, dv_end_rodata; 
extern dv_u32_t dv_start_pgtbl_c0, dv_end_pgtbl_c0;
extern dv_u32_t dv_start_stack_c0, dv_end_stack_c0;
extern dv_u32_t dv_start_data_c0, dv_end_bss_c0;
extern dv_u32_t dv_c0_pages, dv_end_ram;


/* dv_init_memory_management() - initialise the memory management systems
*/
void dv_init_memory_management(dv_kernel_t *kvars)
{
	dv_u32_t p;
	int i;

	DV_DBG(dv_kprintf("dv_init_memory_management()\n"));

	dv_init_pagetable(kvars);

	/* Map a page for the exception vectors.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map page for vectors\n"));
	dv_mmu_map_page(kvars, (void *)DV_VECTOR_LOCATION, (void *)DV_VECTOR_LOCATION, DV_L1_ATTR, DV_L2_ATTR_V);

	/* Map code pages for the text region.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for text\n"));
	for ( p = (dv_u32_t)&dv_start_text; p < (dv_u32_t)&dv_end_text; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_C);
	}

	/* Map read-only pages (no execute) for the rodata region.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for rodata\n"));
	for ( p = (dv_u32_t)&dv_start_rodata; p < (dv_u32_t)&dv_end_rodata; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_RO);
	}

	/* Map read/write pages for the page table region.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for L1 page table\n"));
	for ( p = (dv_u32_t)&dv_start_pgtbl_c0; p < (dv_u32_t)&dv_end_pgtbl_c0; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_RW);
	}
	
	/* Map read/write pages for the stack region.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for kernel stack\n"));
	for ( p = (dv_u32_t)&dv_start_stack_c0; p < (dv_u32_t)&dv_end_stack_c0; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_RW);
	}

	/* Map read/write pages for the data/bss region.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for kernel data/bss\n"));
	for ( p = (dv_u32_t)&dv_start_data_c0; p < (dv_u32_t)&dv_end_bss_c0; p += DV_MEM_PAGESIZE )
	{
		dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_RW);
	}

	/* Map pages for the I/O regions (int. controller, UART, timer, GPIO etc.)
	 * UART is in the AUX page.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map page for I/O aux\n"));
	dv_mmu_map_page(kvars, (void *)&dv_arm_bcm2835_aux, (void *)&dv_arm_bcm2835_aux, DV_L1_ATTR, DV_L2_ATTR_IO);
	DV_DBG(dv_kprintf("dv_init_memory_management() - map page for I/O gpio\n"));
	dv_mmu_map_page(kvars, (void *)&dv_arm_bcm2835_gpio, (void *)&dv_arm_bcm2835_gpio, DV_L1_ATTR, DV_L2_ATTR_IO);
	DV_DBG(dv_kprintf("dv_init_memory_management() - map page for I/O interrupt controller\n"));
	dv_mmu_map_page(kvars, (void *)&dv_arm_bcm2835_interruptcontroller, (void *)&dv_arm_bcm2835_interruptcontroller,
																							DV_L1_ATTR, DV_L2_ATTR_IO);
	DV_DBG(dv_kprintf("dv_init_memory_management() - map page for I/O timer\n"));
	dv_mmu_map_page(kvars, (void *)&dv_arm_bcm2835_systimer, (void *)&dv_arm_bcm2835_systimer,
																							DV_L1_ATTR, DV_L2_ATTR_IO);

	/* Map pages for the page tables.
	*/
	DV_DBG(dv_kprintf("dv_init_memory_management() - map pages for L2 page tables\n"));
	for ( i = 0; i < DV_ARMV6MMU_L1_SIZE; i++ )
	{
		if ( kvars->cpu.page_table->l1page[i] != 0 )
		{
			p = kvars->cpu.page_table->l1page[i] & DV_V6MMUL1_L2B_ADDR;
			dv_mmu_map_page(kvars, (void *)p, (void *)p, DV_L1_ATTR, DV_L2_ATTR_RW);
		}
	}

	/* Caches not enabled yet, so no need to flush/invalidate/disable here.
	 * N is zero, so all translations use TTB0.
	*/
	DV_dbg(dv_kprintf("dv_init_memory_management() - set ttb0 to 0x%08x\n", (dv_u32_t)kvars->cpu.page_table));
	dv_set_translation_table_base_0((dv_u32_t)kvars->cpu.page_table);
	DV_dbg(dv_kprintf("dv_init_memory_management() - set all domain access control to \"manager\"\n"));
	dv_set_domain_access_control(0x55555555);		/* Everything to "client" */
	DV_dbg(dv_kprintf("dv_init_memory_management() - enable MMU in armv6 mode\n"));
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_M | DV_CP15_CTRL_XP);
	DV_dbg(dv_kprintf("dv_init_memory_management() - MMU should be enabled now\n"));
	kvars->cpu.mmu_active = 1;

}
