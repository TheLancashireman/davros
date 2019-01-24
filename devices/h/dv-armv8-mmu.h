/*	dv-armv8-mmu.h - arm v8 MMU header file for davros
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
#ifndef dv_armv8_mmu_h
#define dv_armv8_mmu_h	1

#include <dv-devices.h>

#if !DV_ASM

/* Bits in SCTLR_EL1
*/
#define DV_SCTLR_M			0x0000000000000001uL	/* MMU enable */
#define DV_SCTLR_A			0x0000000000000002uL	/* Alignment check enable */
#define DV_SCTLR_C			0x0000000000000004uL	/* Data cache enable */
#define DV_SCTLR_SA			0x0000000000000008uL	/* Stack alignment check enable */
#define DV_SCTLR_SA0		0x0000000000000010uL	/* Stack alignment check enable at EL0 */
#define DV_SCTLR_I			0x0000000000001000uL	/* Instruction cache enable */
#define DV_SCTLR_WXN		0x0000000000080000uL	/* Write implies no execute */
#define DV_SCTLR_E0E		0x000000000100000uL		/* Endianness at EL0 (1 = big-endian) */
#define DV_SCTLR_EE			0x000000000200000uL		/* Endianness at EL1 (1 = big-endian) */
#define DV_SCTLR_LSMAOE		0x0000000020000000uL	/* Store multiple at aa32 is atomic */
#define	DV_SCTLR_NTLMSD		0x0000000010000000uL	/* Store mutliple at aa32 traps if device memory */

/* Bits in TCR_EL1
*/
#define DV_TCR_T0SZ			0x000000000000003fuL	/* Size of region for TTBR0 : 2**(64-T0SZ) */
#define DV_TCR_EPD0			0x0000000000000080uL	/* TLB miss for translations using TTBR0 */
#define DV_TCR_IRGN0		0x0000000000000300uL	/* Inner cacheability for tt walks using TTBR0 */
#define DV_TCR_ORGN0		0x0000000000000c00uL	/* Outer cacheability for tt walks using TTBR0 */
#define DV_TCR_SH0			0x0000000000003000uL	/* Shareability for tt walks using TTBR0 */
#define DV_TCR_TG0			0x000000000000c000uL	/* Granule size for TTBR0 */
#define DV_TCR_T1SZ			0x00000000003f0000uL	/* Size of region for TTBR1 : 2**(64-T1SZ) */
#define DV_TCR_A1			0x0000000000400000uL	/* ASID select: 1 = TTBR1.ASID, 0 = TTBR0 ASID */
#define DV_TCR_EPD1			0x0000000000800000uL	/* TLB miss for translations using TTBR1 */
#define DV_TCR_IRGN1		0x0000000003000000uL	/* Inner cacheability for tt walks using TTBR1 */
#define DV_TCR_ORGN1		0x000000000c000000uL	/* Outer cacheability for tt walks using TTBR1 */
#define DV_TCR_SH1			0x0000000030000000uL	/* Shareability for tt walks using TTBR1 */
#define DV_TCR_TG1			0x00000000c0000000uL	/* Granule size for TTBR1 */
#define DV_TCR_IPS			0x0000000700000000uL	/* Intermediate physical address size */
#define DV_TCR_AS			0x0000001000000000uL	/* ASID size */
#define DV_TCR_TBI0			0x0000002000000000uL	/* Top byte ignored (used for tagging) in TTBR0 translations */
#define DV_TCR_TBI1			0x0000004000000000uL	/* Top byte ignored (used for tagging) in TTBR1 translations */
#define DV_TCR_HA			0x0000008000000000uL
#define DV_TCR_HD			0x0000010000000000uL
#define DV_TCR_HPD0			0x0000020000000000uL
#define DV_TCR_HPD1			0x0000040000000000uL
#define DV_TCR_TBID0		0x0008000000000000uL
#define DV_TCR_TBID1		0x0010000000000000uL
#define DV_TCR_NFD0			0x0020000000000000uL
#define DV_TCR_NFD1			0x0040000000000000uL

/* Bits in page table descriptors
*/
#define DV_PGT_INVALID		0x0000000000000000uL
#define DV_PGT_VALID		0x0000000000000001uL	/* If 0, other bits ignored */
#define DV_PGT_TABLE		0x0000000000000002uL	/* Descriptor indicates a page table */

/* Bits in page table descriptor when DV_PGT_TABLE is 0 (i.e. a block or page descriptor)
*/
#define DV_PGT_LBA			0x0000000000000ffcuL	/* Lower block attributes */
#define DV_PGT_BBA			0x0000fffffffff000uL	/* Block base address (must be block-aligned) */
#define DV_PGT_UBA			0xfff8000000000000uL	/* Upper block attributes */

/* Bits in page table descriptor when DV_PGT_TABLE is 1, not level 3 (i.e. a page table descriptor)
*/
#define DV_PGT_TBA			0x0000fffffffff000uL	/* Table base address (must be granule-aligned) */
#define DV_PGT_PXN			0x0800000000000000uL
#define DV_PGT_XN			0x1000000000000000uL
#define DV_PGT_AP			0x6000000000000000uL
#define DV_PGT_NS			0x8000000000000000uL

/* Bits in the attribute fields
*/
#define DV_ATTR_PBHA		0x7800000000000000uL	/* Page-based hardware attributes */
#define DV_ATTR_UXN			0x0040000000000000uL	/* (Unprivileged) execute never */
#define DV_ATTR_PXN			0x0020000000000000uL	/* Privileged execute never (only for TTBR1 addresses) */
#define DV_ATTR_CTG			0x0010000000000000uL	/* Contiguous */
#define DV_ATTR_DBM			0x0008000000000000uL	/* Dirty bit modifier */
#define DV_ATTR_NG			0x0000000000000800uL	/* Not global */
#define DV_ATTR_AF			0x0000000000000400uL	/* Access flag */
#define DV_ATTR_SH			0x0000000000000300uL	/* Shareability */
#define DV_ATTR_SH_NON		0x0000000000000000uL	/*	- Non-shareable */
#define DV_ATTR_SH_OUTER	0x0000000000000200uL	/*	- Outer Shareable */
#define DV_ATTR_SH_INNER	0x0000000000000300uL	/*	- Inner Shareable */
#define DV_ATTR_AP			0x00000000000000c0uL	/* Data access permissions */
#define DV_ATTR_AP_RW___	0x0000000000000000uL	/*	- S: RW, U: - */
#define DV_ATTR_AP_RW_RW	0x0000000000000040uL	/*	- S: RW, U: RW */
#define DV_ATTR_AP_R____	0x0000000000000080uL	/*	- S: RW, U: - */
#define DV_ATTR_AP_R__R_	0x00000000000000c0uL	/*	- S: RW, U: R */
#define DV_ATTR_NS			0x0000000000000020uL	/* Non-secure */
#define DV_ATTR_AIND		0x000000000000001cuL	/* Attribute index */

/* Some structures to represent memory spaces
*/
typedef struct dv_4KiBpage_s
{
	dv_u64_t m[(4*1024)/sizeof(dv_u64_t)];
} dv_4KiBpage_t;

typedef struct dv_2MiBkpage_s
{
	dv_u64_t m[(2*1024*1024)/sizeof(dv_u64_t)];
} dv_2MiBpage_t;

/* Bits in each field of the MAIR_EL1 register
 *
 * All 8 bits for device space
 * Any other values with the upper 4 bits zero are unpredictable
*/
#define DV_MAIR_DEV_NGNRNE	0x00
#define DV_MAIR_DEV_NGNRE	0x04
#define DV_MAIR_DEV_NGRE	0x08
#define DV_MAIR_DEV_GRE		0x0c

/* Bits in each field of the MAIR_EL1 register
 *
 * Upper 4 bits for memory space define the outer cacheability.
 * Use DV_MAIR_ONC, or combine one DV_MAIR_OW* with one or both DV_MAIR_OA*
*/
#define DV_MAIR_ONC			0x40	/* Outer non-cacheable */

#define DV_MAIR_OAW			0x10	/* Allocate on write */
#define DV_MAIR_OAR			0x20	/* Allocate on read */

#define DV_MAIR_OWTT		0x00	/* Outer write-through, transient */
#define DV_MAIR_OWBT		0x40	/* Outer write-back, transient */
#define DV_MAIR_OWTN		0x80	/* Outer write-through, non-transient */
#define DV_MAIR_OWBN		0xc0	/* Outer write-back, non-transient */

/* Bits in each field of the MAIR_EL1 register
 *
 * Lower 4 bits for memory space define the inner cacheability. Use in combination with the outer cacheability
 * Use DV_MAIR_INC, or combine one  DV_MAIR_IW* with one or both DV_MAIR_IA*
*/
#define DV_MAIR_INC			0x04	/* Inner non-cacheable */

#define DV_MAIR_IAW			0x01	/* Allocate on write */
#define DV_MAIR_IAR			0x02	/* Allocate on read */

#define DV_MAIR_IWTT		0x00	/* Inner write-through, transient */
#define DV_MAIR_IWBT		0x04	/* Inner write-back, transient */
#define DV_MAIR_IWTN		0x08	/* Inner write-through, non-transient */
#define DV_MAIR_IWBN		0x0c	/* Inner write-back, non-transient */

static inline void dv_set_mair_field(int idx, dv_u64_t val)
{
	dv_u64_t mair = dv_arm64_mrs(MAIR_EL1);
	dv_u64_t mask = 0xff;
	int shift = idx*8;

	mask = mask << shift;
	mair &= ~mask;
	mair |= val << shift;

	dv_arm64_msr(MAIR_EL1, mair);
}

extern void dv_armv8_mmu_setup(void);
extern void dv_setMMUregisters(dv_u64_t ttbr0, dv_u64_t mair, dv_u64_t tcr, dv_u64_t sctlr, dv_u64_t ttbr1);

#endif

#endif
