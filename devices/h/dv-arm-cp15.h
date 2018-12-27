/*	dv-arm-cp15.h - arm CP15 control
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
#ifndef dv_arm_cp15_h
#define dv_arm_cp15_h	1

#include <dv-devices.h>

/*
 * mcr = "send command to coprocessor and pass some data in"
 * mrc = "send command to coprocessor and get some data back"
 *
 * For CP15:
 *	mcr	P15, Op1, Rd, CRn, CRm, Op2
 *	mrc	P15, Op1, Rd, CRn, CRm, Op2
 *
 * Rd = CPU register
 * CRn = register in CP15
 * Op1 = Opcode_1 value for the register
 * CRm = operational register
 * Op2 = Opcode_2 value for the register
*/

/* Bits in these registers
*/
#define DV_CP15_CTRL_FA		0x20000000		/* Force AP */
#define DV_CP15_CTRL_TR		0x10000000		/* TEX remap */
#define DV_CP15_CTRL_EE		0x02000000		/* CPSR.E after exception */
#define DV_CP15_CTRL_VE		0x01000000		/* VIC for interrupt vectors */
#define DV_CP15_CTRL_XP		0x00800000		/* Extended page tables */
#define DV_CP15_CTRL_U		0x00400000		/* Unaligned access */
#define DV_CP15_CTRL_FI		0x00200000		/* Low latenct for fast interrupts */
#define DV_CP15_CTRL_IT		0x00040000		/* Deprecated - should be 1 */
#define DV_CP15_CTRL_DT		0x00010000		/* Deprecated - should be 1 */
#define DV_CP15_CTRL_L4		0x00008000		/* Loading PC sets T bit (thumb?) */
#define DV_CP15_CTRL_RR		0x00004000		/* Random cache replacement */
#define DV_CP15_CTRL_V		0x00002000		/* High vector location */
#define DV_CP15_CTRL_I		0x00001000		/* Enable L1 instruction cache */
#define DV_CP15_CTRL_Z		0x00000800		/* Enable branch prediction */
#define DV_CP15_CTRL_F		0x00000400		/* Should be zero */
#define DV_CP15_CTRL_R		0x00000200		/* Deprecated - ROM protection */
#define DV_CP15_CTRL_S		0x00000100		/* Deprecated - MMU protection */
#define DV_CP15_CTRL_B		0x00000080		/* Big-endian */
#define DV_CP15_CTRL_W		0x00000008		/* Not implemented */
#define DV_CP15_CTRL_C		0x00000004		/* Enable L1 data cache */
#define DV_CP15_CTRL_A		0x00000002		/* Enable strict alignment - takes priority over U */
#define DV_CP15_CTRL_M		0x00000001		/* MMU enable */

#define DV_CP15_ACTL_FIO	0x80000000		/* Fast interrupt override */
#define DV_CP15_ACTL_FSD	0x40000000		/* Disable speculative operations */
#define DV_CP15_ACTL_BFD	0x20000000		/* Disable branch folding */
#define DV_CP15_ACTL_PHD	0x10000000		/* Disable prefetch halting */
#define DV_CP15_ACTL_CZ		0x00000040		/* Cache size restriction */
#define DV_CP15_ACTL_RV		0x00000020		/* Disable cache block transfer */
#define DV_CP15_ACTL_RA		0x00000010		/* Disable clean entire cache */
#define DV_CP15_ACTL_TR		0x00000008		/* MicroTLB replacement */
#define DV_CP15_ACTL_SB		0x00000004		/* Static branch prediction if Z */
#define DV_CP15_ACTL_DB		0x00000002		/* Dynamic branch prediction if Z */
#define DV_CP15_ACTL_RS		0x00000001		/* Return stack if Z */

/* CP15 control registers
*/
static inline dv_u32_t dv_read_cp15_control(void)
{
	dv_u32_t x;
	__asm__ volatile ("mrc  p15, 0, %0, c1, c0, 0" : "=r"(x) : );
	return x;
}

static inline void dv_write_cp15_control(dv_u32_t x)
{
	__asm__ volatile ("mcr  p15, 0, %0, c1, c0, 0" : : "r"(x) : );
}

static inline dv_u32_t dv_read_cp15_aux_control(void)
{
	dv_u32_t x;
	__asm__ volatile ("mrc  p15, 0, %0, c1, c0, 1" : "=r"(x) : );
	return x;
}

static inline void dv_write_cp15_aux_control(dv_u32_t x)
{
	__asm__ volatile ("mcr  p15, 0, %0, c1, c0, 1" : : "r"(x) : );
}

#endif
