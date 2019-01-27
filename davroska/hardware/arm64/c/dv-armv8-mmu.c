/*	dv-armv8-mmu.c - arm v8 MMU setup for davroska 
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
#define DV_ASM	0
#include <dv-config.h>
#include <dv-devices.h>
#include <dv-armv8-mmu.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif
#undef DV_DEBUG
#define DV_DEBUG 1

/* davroska doesn't support memory protection or virtualized addresses.
 * So the MMU setup is fixed at startup.
 *
 * Assumptions for now:
 *	- the 4 KiB granule size results in the least memory for page tables
 *	- L0 table is not used because the address range is restricted
 *	- L1 table has 512 entries:
 *		- entry 0 points to the level 2 table (0x00000000 <= address < 0x40000000)
 *		- entry 1 is a block of device memory (0x40000000 <= address < 0x80000000)
 *		- the remaining entries are "invalid"
 *	- L2 table has 512 entries - fully populated. Each entry represents 2 MiB
 *		- entries 0 to 503 are blocks of cacheable memory (0x00000000 <= address < 0x3f000000)
 *		- entries 504 to 511 are blocks of device memory (0x3f000000 <= address < 0x40000000)
 *	- L3 tables are not needed because all level 2 entries are blocks
 *
 * The "kernel space" (upper address bits all 1) is not used (TCR_EL1.EPD1 = 1) so the value of
 * TTBR1_EL1 is irrelevant.
 *
 * The three tables are allocated (correctly aligned) in the linker script.
 *
*/
extern dv_4KiBpage_t dv_l1_table;
extern dv_4KiBpage_t dv_l2_table;

/* ToDo: this is quite specific to the raspberry pi 3
*/
extern dv_2MiBpage_t dv_memory[];			/* Memnory - ends at dv_peripheral1 */
extern dv_2MiBpage_t dv_peripheral1[];		/* BCM2835 peripherals - ends at dv_peripheral2 */
extern dv_2MiBpage_t dv_peripheral2[];		/* BCM2836 peripherals */

volatile dv_boolean_t tables_ready;

/* MAIR attributes
 *	- device : MAIR index 0, nG, nR, nE
 *	- memory : MAIR index 1, outer and inner cacheable, allocate on read and write.
*/
#define DV_MAIR_DEV			0
#define DV_MAIR_ATTR_DEV	( DV_MAIR_DEV_NGNRNE )
#define DV_MAIR_MEM			1
#define DV_MAIR_ATTR_MEM	((DV_MAIR_OWTN | DV_MAIR_OAW | DV_MAIR_OAR) | (DV_MAIR_IWTN | DV_MAIR_IAW | DV_MAIR_IAR))

/* Page attributes
 *	- memory : outer shared
 *			 : supervisor RW, user no access. Have to have this, otherwise there's no execute at EL1
 *			   (see https://community.arm.com/processors/f/discussions/11841/mmu---permission-fault-with-el1-access)
 *			 : AF = 1 (otherwise we get a trap to "load" the page)
 *			 : MAIR index = memory attribute field
 *	- device : MAIR index = device attribte field
 *			 : execute never (both privileged andf unprivileged
 *			 : otherwise same as memory, although the "device" MAIR setting tells it to ignore most of them)
*/
#define DV_PG_ATTR_MEM		( DV_ATTR_SH_OUTER | \
							  DV_ATTR_AP_RW___ | \
							  DV_ATTR_AF | \
							  (DV_MAIR_MEM << 2) )
#define DV_PG_ATTR_DEV		( DV_ATTR_UXN | DV_ATTR_PXN | \
							  DV_ATTR_SH_OUTER | \
							  DV_ATTR_AP_RW___ | \
							  DV_ATTR_AF | \
							  (DV_MAIR_DEV << 2) )

/* dv_armv8_mmu_setup() - set up the MMU
 *
 *	- intialises the page tables (only if init_tables is true)
 *	- configures the MMU to use the page tables
 *
 * The function is called with the parameter = true on the davroska core.
 * The other cores wait until the variable "tables_ready" is true,
 * then call this function with the parameter = false to set up their own MMU
*/
void dv_armv8_mmu_setup(dv_boolean_t init_tables)
{
	if ( init_tables )
	{
#if DV_DEBUG
		dv_printf("dv_armv8_mmu_setup() - initialize page table\n");
		dv_printf("No. of memory blocks: %d\n", &dv_peripheral1[0] - &dv_memory[0]);
		dv_printf("No. of peripheral1 blocks: %d\n", &dv_peripheral2[0] - &dv_peripheral1[0]);
#endif

		/* Initialise the two valid entries in the L1 table.
		 *	- Entry 0 points to an L2 table
		 *	- Entry 1 is a 1 GiB block that covers the BCM2836 peripherals
		 *		Note: the peripheral space is much smaller than 1 GiB, but the rest is empty
		*/
		dv_l1_table.m[0] = DV_PGT_VALID | DV_PGT_TABLE | DV_PG_ATTR_MEM | (dv_u64_t)&dv_l2_table;
		dv_l1_table.m[1] = DV_PGT_VALID | DV_PG_ATTR_DEV | (dv_u64_t)&dv_peripheral2[0];

		/* Initialise the rest of the L1 table to "invalid"
		*/
		for ( int i = 2; i < 512; i++ )
		{
			dv_l1_table.m[i] = DV_PGT_INVALID;
		}

		/* Initialise the "memory" entries" of the L2 table
		*/
		for ( int i = 0; i < 504; i++ )
		{
			dv_l2_table.m[i] = DV_PGT_VALID | DV_PG_ATTR_MEM | (dv_u64_t)&dv_memory[i];
		}

		/* Initialise the "device" entries" of the L2 table
		*/
		for ( int i = 0; i < 8; i++ )
		{
			dv_l2_table.m[504+i] = DV_PGT_VALID | DV_PG_ATTR_DEV | (dv_u64_t)&dv_peripheral1[i];
		}

		tables_ready = 1;
	}
	else
	{
		while ( tables_ready == 0 )
		{
			/* Wait for core 0 to do its job
			*/
		}
	}

	/* Set the parameters that we need in TCR_EL1
	*/
	dv_u64_t tcr = 0;
	tcr |= 25uL;				/* T0SZ -> restrict addresses to 39-bits */
								/* EPD0 = 0 -> TTBR0 translation enabled */
	tcr |= 1uL << 8;			/* IRGN0 = write-back */
	tcr	|= 1uL << 10;			/* ORGN0 = write-back */
	tcr |= 3uL << 12;			/* SH0 = inner shareable */
								/* TG0 = 0 -> 4k pages */
	tcr |= 25uL << 16;			/* T1SZ -> restrict addresses to 39-bits */
	tcr |= DV_TCR_EPD1;			/* EPD1 -> disable "kernel" space */
	tcr |= 1uL << 24;			/* IRGN1 = write-back */
	tcr	|= 1uL << 26;			/* ORGN1 = write-back */
	tcr |= 3uL << 28;			/* SH1 = inner shareable */
	tcr |= 2uL << 30;			/* TG1 = 2 ->> 4k pages (different encoding from TG0) */
	tcr |= 2uL << 32;			/* IPS = 2 (40-bit intermediate) */
								/* AS = 0 -> 8-bit ASID */
								/* TBI0 = TBI1 = 0 -> tagging disabled */
	
	dv_u64_t sctlr = dv_arm64_mrs(SCTLR_EL1);
	sctlr |= DV_SCTLR_M;		/* Enable MMU */
	sctlr |= DV_SCTLR_C;		/* Enable data cache */
#if 0
	sctlr &= ~DV_SCTLR_C;		/* Temp: disable cache */
#endif
	sctlr |= DV_SCTLR_I;		/* Enable instruction cache */
	sctlr |= DV_SCTLR_SA;		/* Stack alignment check */
	sctlr |= DV_SCTLR_SA0;		/* Stack alignment check at EL0 */
	sctlr |= DV_SCTLR_LSMAOE;	/* Store multiple (aarch32) is atomic */
	sctlr |= DV_SCTLR_NTLMSD;	/* Store multiple device (aarch32) is trapped */
	sctlr &= ~DV_SCTLR_WXN;		/* Allow to execute writeable pages */
	sctlr &= ~DV_SCTLR_UWXN;	/* Allow to execute EL0-writeable pages at EL1 */
	sctlr &= ~DV_SCTLR_E0E;		/* Little endian at EL0 */
	sctlr &= ~DV_SCTLR_EE;		/* Little endian at EL1 (incl. translation tables) */
	sctlr |= (1<<20);			/* Bit 20 is RES1 but reads zero */
	sctlr |= 0xc00800;			/* RES1 bits */

	dv_u64_t mair = (DV_MAIR_ATTR_MEM << (DV_MAIR_MEM * 8)) | (DV_MAIR_ATTR_DEV << (DV_MAIR_DEV * 8));

	dv_u64_t ttbr0 = (dv_u64_t)&dv_l1_table;
	ttbr0 |= 0x01uL;

	dv_u64_t ttbr1 = ttbr0;

	if ( init_tables )
	{
#if DV_DEBUG
		dv_printf("L1 0x%016lx   0 : 0x%016lx\n", (dv_u64_t)&dv_l1_table, dv_l1_table.m[0]);
		dv_printf("L1 0x%016lx   1 : 0x%016lx\n", (dv_u64_t)&dv_l1_table, dv_l1_table.m[1]);
		dv_printf("L1 0x%016lx   2 : 0x%016lx\n", (dv_u64_t)&dv_l1_table, dv_l1_table.m[2]);
		dv_printf("L2 0x%016lx   0 : 0x%016lx\n", (dv_u64_t)&dv_l2_table, dv_l2_table.m[0]);
		dv_printf("L2 0x%016lx 503 : 0x%016lx\n", (dv_u64_t)&dv_l2_table, dv_l2_table.m[503]);
		dv_printf("L2 0x%016lx 504 : 0x%016lx\n", (dv_u64_t)&dv_l2_table, dv_l2_table.m[504]);
		dv_printf("L2 0x%016lx 511 : 0x%016lx\n", (dv_u64_t)&dv_l2_table, dv_l2_table.m[511]);

		dv_printf("ttbr0  = 0x%016lx\n", ttbr0);
		dv_printf("ttbr1  = 0x%016lx\n", ttbr1);
		dv_printf("mair  = 0x%016lx\n", mair);
		dv_printf("tcr   = 0x%016lx\n", tcr);
		dv_printf("sctlr = 0x%016lx\n", sctlr);
#endif
	}

	dv_setMMUregisters(ttbr0, mair, tcr, sctlr, ttbr1);
}
