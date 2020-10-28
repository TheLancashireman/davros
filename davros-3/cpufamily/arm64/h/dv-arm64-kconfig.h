/*	dv-arm64-kconfig.h - kernel configuration for arm64 processors
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
#ifndef dv_arm64_kconfig_h
#define dv_arm64_kconfig_h	1

/* Include files for ARM.
*/
#define DV_H_REGISTERS		<dv-arm64-core.h>
#define DV_H_DISPATCH		<cpufamily/arm64/h/dv-arm64-dispatch.h>
#define DV_H_START			<cpufamily/arm64/h/dv-arm64-start.h>
#define DV_H_MMU			<dv-armv8-mmu.h>

/* 64-bit processor with no oddities.
*/
#include <dv-types-64.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>

/* Size of kernel stack in stackwords.
*/
#define DV_KSTACK_WORDS		(DV_MEM_PAGESIZE/sizeof(dv_stackword_t))

/* Parameters for memory regions, including stacks.
 *
 * DV_MEM_PAGESIZE - granularity of memory management blocks.
 * DV_MEM_GAP - minimum gap to leave at bottom of block.
*/
#define DV_MEM_PAGESIZE	4096	/* ARM MMU page size */
#define DV_MEM_GAP		0		/* ARM MMU doesn't allow multi-byte accesses to spill */

/* Vector table location. For ARM processors that have RAM at address 0 we copy the vectors
 * there.
 * If there's read-only memory at 0 we have to flash a vector table that redirects to somewhere in
 * RAM, or maybe there's a vector base address register.
*/
#define DV_VECTOR_LOCATION		0

#if !DV_ASM

/* CPU-specific kernel variables.
*/
#define DV_CPU_HAS_KVARS		1
#define DV_CPU_KVARS_TYPE		dv_arm64_kvars_t

typedef struct dv_arm64_kvars_s dv_arm64_kvars_t;
typedef struct dv_armv6_l1pagetable_s dv_armv6_l1pagetable_t;

struct dv_arm64_kvars_s
{
	dv_armv6_l1pagetable_t *page_table;
	dv_mempage_t *l2_table_page;
	dv_boolean_t mmu_active;
};

static inline void dv_hw_wait(void)
{
	__asm__ volatile ("wfi");
}

#endif

#endif
