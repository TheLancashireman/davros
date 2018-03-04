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

#define L1_ATTR		(DV_V6MMUL1_L2B | DV_V6MMUL1_NS)
#define L2_ATTR_C	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1)
#define L2_ATTR_D	(DV_V6MMUL2_PB|DV_V6MMUL2_B|DV_V6MMUL2_C|DV_V6MMUL2_AP_1|DV_V6MMUL2_XN)
        



/* dv_init_memory_management() - initialise the memory management systems
*/
void dv_init_memory_management(dv_kernel_t *kvars)
{
	dv_kprintf("dv_init_memory_management()\n");

	dv_init_mmu(kvars);

	/* Map a page for the exception vectors.
	*/
	dv_mmu_map_page(kvars, DV_VECTOR_LOCATION, DV_VECTOR_LOCATION, L1_ATTR, L2_ATTR_C);
}
