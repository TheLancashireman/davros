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

/* Bits in TCR_EL1
*/
#define DV_TCR_T0SZ			0x000000000000003f		/* Size of region for TTBR0 : 2**(64-T0SZ) */
#define DV_TCR_EPD0			0x0000000000000080		/* TLB miss for translations using TTBR0 */
#define DV_TCR_IRGN0		0x0000000000000300		/* Inner cacheability for tt walks using TTBR0 */
#define DV_TCR_ORGN0		0x0000000000000c00		/* Outer cacheability for tt walks using TTBR0 */
#define DV_TCR_SH0			0x0000000000003000		/* Shareability for tt walks using TTBR0 */
#define DV_TCR_TG0			0x000000000000c000		/* Granule size for TTBR0 */
#define DV_TCR_T1SZ			0x00000000003f0000		/* Size of region for TTBR1 : 2**(64-T1SZ) */
#define DV_TCR_A1			0x0000000000400000		/* ASID select: 1 = TTBR1.ASID, 0 = TTBR0 ASID */
#define DV_TCR_EPD1			0x0000000000800000		/* TLB miss for translations using TTBR1 */
#define DV_TCR_IRGN1		0x0000000003000000		/* Inner cacheability for tt walks using TTBR1 */
#define DV_TCR_ORGN1		0x000000000c000000		/* Outer cacheability for tt walks using TTBR1 */
#define DV_TCR_SH1			0x0000000030000000		/* Shareability for tt walks using TTBR1 */
#define DV_TCR_TG1			0x00000000c0000000		/* Granule size for TTBR1 */
#define DV_TCR_IPS			0x0000000700000000		/* Intermediate physical address size */
#define DV_TCR_AS			0x0000001000000000		/* ASID size */
#define DV_TCR_TBI0			0x0000002000000000		/* Top byte ignored (used for tagging) in TTBR0 translations */
#define DV_TCR_TBI1			0x0000004000000000		/* Top byte ignored (used for tagging) in TTBR1 translations */
#define DV_TCR_HA			0x0000008000000000
#define DV_TCR_HD			0x0000010000000000
#define DV_TCR_HPD0			0x0000020000000000
#define DV_TCR_HPD1			0x0000040000000000
#define DV_TCR_TBID0		0x0008000000000000
#define DV_TCR_TBID1		0x0010000000000000
#define DV_TCR_NFD0			0x0020000000000000
#define DV_TCR_NFD1			0x0040000000000000

/* Bits in page table descriptors
*/
#define DV_PGT_VALID		0x0000000000000001		/* If 0, other bits ignored */
#define DV_PGT_TABLE		0x0000000000000002		/* Descriptor indicates a page table */

/* Bits in page table descriptor when DV_PGT_TABLE is 0 (i.e. a block descriptor)
*/
#define DV_PGT_LBA			0x0000000000000ffc		/* Lower block attributes */
#define DV_PGT_BBA			0x0000fffffffff000		/* Block base address (must be block-aligned) */
#define DV_PGT_UBA			0xfff0000000000000		/* Upper block attributes */

/* Bits in page table descriptor when DV_PGT_TABLE is 1, not level 3 (i.e. a page table descriptor)
*/
#define DV_PGT_TBA			0x0000fffffffff000		/* Table base address (must be granule-aligned) */
#define DV_PGT_PXN			0x0800000000000000
#define DV_PGT_XN			0x1000000000000000
#define DV_PGT_AP			0x6000000000000000
#define DV_PGT_NS			0x8000000000000000

/* Bits in page table descriptor when DV_PGT_TABLE is 1, at level 3 (i.e. a page descriptor)
*/
#define DV_PGT_LPA			0x0000000000000ffc		/* Lower page attributes */
#define DV_PGT_BBA			0x0000fffffffff000		/* Page base address (must be granule-aligned) */
#define DV_PGT_UBA			0xfff8000000000000		/* Upper page attributes */

/* Bits in the attribute fields
*/
#define DV_ATTR_PBHA		0x7800000000000000		/* Page-based hardware attributes */
#define DV_ATTR_UXN			0x0040000000000000		/* (Unprivileged) execute never */
#define DV_ATTR_PXN			0x0020000000000000		/* Privileged execute never (only for TTBR1 addresses) */
#define DV_ATTR_CTG			0x0010000000000000		/* Contiguous */
#define DV_ATTR_DBM			0x0008000000000000		/* Dirty bit modifier */
#define DV_ATTR_NG			0x0000000000000800		/* Not global */
#define DV_ATTR_AF			0x0000000000000400		/* Access flag */
#define DV_ATTR_SH			0x0000000000000300		/* Shareability */
#define DV_ATTR_AP			0x00000000000000c0		/* Data access permissions */
#define DV_ATTR_NS			0x0000000000000020		/* Non-secure */
#define DV_ATTR_AIND		0x000000000000001c		/* Attribute index */

#endif

#endif
