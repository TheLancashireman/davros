/* Cache information
 *
 * CLIDR_EL1 - maybe not useful
 * CCSIDR_EL1 - read out NumSets, Associativity and LineSize of "currently selected" cache
 * CSSELR_EL1 - write this register to choose which cache CCSIDR_EL1 gives information about.
 *
 * https://developer.arm.com/docs/den0024/latest/caches/cache-maintenance
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/

/*
Information:

	CCSIDR_EL1  | 27 ... 13 | 12 ...  3 | 2 1 0 |
                  MaxSet      MaxWay      LineSize
                                            0 -> 16, 1 -> 32, 2 -> 64 etc.

	CCSIDR_EL1 (L1 data)  = 0x700fe01a
    CCSIDR_EL1 (L1 instr) = 0x201fe00a
    CCSIDR_EL1 (L2)       = 0x703fe07a

	Sets: 0x0fffe000  >> 13
    Ways: 0x00001ff8  >> 3
	Line: 0x00000003
*/

#include <arm64/h/dv-arm64-registers.h>

extern void dv_invalidate_setway(dv_u64_t setway);

dv_u64_t l1d, l1i, l2;

dv_u32_t l1d_sets;
dv_u32_t l1d_assoc;
dv_u32_t l1d_lsize;

dv_u32_t l2_sets;
dv_u32_t l2_assoc;
dv_u32_t l2_lsize;

const unsigned log2tbl[10] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

unsigned log2(unsigned x)
{
	for ( unsigned log = 0; log < 10; log++ )
	{
		if ( x < log2tbl[log] )
			return log;
	}

	for (;;)
	{
		/* Help! log2tbl is too small */
	}
}


void get_cache_sizes(void)
{
	/* Let's cheat a bit here; we know that the pi3 has
	 *	- an L1 instruction cache,
	 *	- an L1 data cache,
	 *	- an L2 unified cache
	 * and nothing else... (?)
	 * If this assumption turns out to be wrong, ...
	*/

	dv_arm64_msr(CSSELR_EL1, 0);	/* L1 data cache */
	l1d = dv_arm64_msr(CCSIDR_EL1);
	dv_arm64_msr(CSSELR_EL1, 1);	/* L1 instruction cache */
	l1i = dv_arm64_msr(CCSIDR_EL1);
	dv_arm64_msr(CSSELR_EL1, 2);	/* L2 unified cache */
	l2 = dv_arm64_msr(CCSIDR_EL1);

	l1d_lsize	= l1d & 0x03;				/* 0 --> 16, 1 --> 32 etc. We expect 2 here */
	l1d_assoc	= ((l1d & 0x00001ff8) >> 3)+1;
	l1d_sets	= ((l1d & 0x0fffe000) >> 12)+1;

	l2_lsize	= l2 & 0x03;				/* 0 --> 16, 1 --> 32 etc. We expect 2 here */
	l2_assoc	= ((l2 & 0x00001ff8) >> 3)+1;
	l2_sets		= ((l2 & 0x0fffe000) >> 12+1;
}


void dv_invalidatecache(dv_u32_t level)
{
	dv_u64_t setway = (dv_u64_t)((level-1)<<1);
	dv_u64_t set, way;
	dv_u32_t nsets, nways, setshift, wayshift;

	if ( level == 1 )
	{
		nsets = l1d_sets;
		nways = l1d_assoc;
	}
	else
	if ( level == 2 )
	{
		nsets = l2_sets;
		nways = l2_assoc;
	}
	else
	{
		return;		/* Bummer */
	}

	for ( set = 0; set < nsets; set++ )
	{
		for ( way = 0; way < nways; way++ )
		{
			dv_invalidate_setway(setwayÂ| (set << setshift) | (way << wayshift));
		}
	}
}

