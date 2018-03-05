/*	dv-arm-v6mmu.h - arm v6 MMU header file for davros
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
#ifndef dv_arm_v6mmu_h
#define dv_arm_v6mmu_h	1

#if !DV_ASM

#include <kernel/h/dv-types.h>

/* A page table consists of an array of 32-bit words, each of which
 * specifies some kind of object.
*/
typedef dv_u32_t dv_arm_pagetableentry_t;

typedef struct dv_armv6_l1pagetable_s dv_armv6_l1pagetable_t;
typedef struct dv_armv6_l2pagetable_s dv_armv6_l2pagetable_t;

#define DV_V6MMU_TYPE		0x00000003		/* Entry type (but see XN bit in L2 */

/* The level 1 page table (whose base address is in the page table base register) is
 * an array of 4096 (2**12) page table entries.
 * Each page table entry represents 1 MiB of the address space and is either:
 *		- "invalid" (0),
 *		- the base of a level 2 page table,
 *		- the base of a 1 MiB section or
 *		- the base of a 16 MiB "supersection".
 * A supersection contains 16 identical entries and must start at an index that is a multiple of 16.
 * In each case, the base is 1 MiB aligned and the unused bits contain assorted access flags.
 *
 * A virtual address is made up of several portions:
 *	- bits: 10987654321098765432109876543210
 *	-       11111111111122222222333333333333
 *	-       111111111111                     index in l1 table (0-4095) --> gives physical address of l2 table
 *  -                   22222222             index in l2 table (0-255)  --> gives physical address of page
 *	-                           333333333333 offset in page
*/
#define DV_ARMV6MMU_L1_SIZE		4096
#define DV_ARMV6MMU_L1_MASK		0xfff00000
#define DV_ARMV6MMU_L2_MASK		0x000ff000
#define DV_ARMV6MMU_OFF_MASK	0x00000fff

static inline dv_u32_t dv_armv6_virt_to_l1(dv_u32_t v)
{
	return v >> 20;
}

static inline dv_u32_t dv_armv6_virt_to_l2(dv_u32_t v)
{
	return (v & DV_ARMV6MMU_L2_MASK) >> 12;
}

struct dv_armv6_l1pagetable_s
{
	dv_arm_pagetableentry_t l1page[DV_ARMV6MMU_L1_SIZE];
};

/* Bitfields in the L1 entries.
*/
#define DV_V6MMUL1_L2B		0x00000001		/* Entry is base of an L2 table */
#define DV_V6MMUL1_NS		0x00000008		/* Non-secure */
#define DV_V6MMUL1_DOM		0x000001e0		/* Domain */
#define DV_V6MMUL1_P		0x00000200		/* P - enables ECC (not supported on 1176) */
#define DV_V6MMUL1_L2B_ADDR	0xfffffc00		/* Base address of L2 table */

/* A level 2 page table (whose base address is in an L1 page table entry) is
 * an array of 256 (2**8) page table entries.
 * Each page table entry represents 4 KiB of the address space and is either:
 *		- "invalid (0),
 *		- the address of a 4 KiB page or
 *		- the address of a 64 KiB "large page".
 * A large page contains 16 identical entries and must start at an index that is a multiple of 16.
 * In each case, the base is 4 KiB aligned and the unused bits contain assorted access flags.
*/
#define DV_ARMV6MMU_L2_SIZE    256

struct dv_armv6_l2pagetable_s
{
	dv_arm_pagetableentry_t l2page[DV_ARMV6MMU_L2_SIZE];
};

/* Bitfields in the L2 entries.
*/
#define DV_V6MMUL2_XN		0x00000001		/* XN - execute never */
#define DV_V6MMUL2_PB		0x00000002		/* This bit is 1 if the entry is a 4k page */
#define DV_V6MMUL2_B		0x00000004		/* B - bufferable */
#define DV_V6MMUL2_C		0x00000008		/* C - cacheable */
#define DV_V6MMUL2_AP		0x00000030		/* AP - access permissions */
#define DV_V6MMUL2_AP_0		0x00000000		/*		no access */
#define DV_V6MMUL2_AP_1		0x00000010		/*		s = r/w  u = none */
#define DV_V6MMUL2_AP_2		0x00000020		/*		s = r/w  u = r */
#define DV_V6MMUL2_AP_3		0x00000030		/*		s = r/w  u = r/w */
#define DV_V6MMUL2_TEX		0x000001c0		/* TEX */
#define DV_V6MMUL2_APX		0x00000200		/* APX : 1 converts s = r/w to s = r in AP */
#define DV_V6MMUL2_S		0x00000400		/* S */
#define DV_V6MMUL2_NG		0x00000800		/* nG */
#define DV_V6MMUL2_ADDR		0xfffff000		/* Physical page address */

void dv_armv6_mmu_init_pagetable(dv_kernel_t *kvars);
void dv_armv6_mmu_map_page(dv_kernel_t *kvars, void *phys, void *virt, dv_u32_t l1_attr, dv_u32_t l2_attr);

/* Map MMU functions to v6 MMU functions
*/
static inline void dv_init_pagetable(dv_kernel_t *kvars)
{
	dv_armv6_mmu_init_pagetable(kvars);
}

static inline void dv_mmu_map_page(dv_kernel_t *kvars, void *phys, void *virt, dv_u32_t l1_attr, dv_u32_t l2_attr)
{
	dv_armv6_mmu_map_page(kvars, phys, virt, l1_attr, l2_attr);
}

#endif

#endif
