/*	dv-arm64-registers.h - ARM64 registers for davroska
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
#ifndef dv_arm64_registers_h
#define dv_arm64_registers_h	1

/* SPSR registers. These registers contain the state of the interrupted thread.
 *
 * When the interrupted thread is aarch32 there are more bits than this.
 *
 * The D, A, I and F bits can also be used in the DAIF pseudo-register
*/
#define DV_SPSR_M	0x0000001f		/* Execution state ... */
#define DV_SPSR_SP	0x00000001			/* Use ELx SP (0 = use EL0 SP) */
#define DV_SPSR_EL	0x0000000c			/* Exception level of interrupted thread */
#define DV_SPSR_32	0x00000010			/* Exception from aarch32 (0 = aarch64) */
#define DV_SPSR_F	0x00000040		/* FIQ mask - 1 to disable */
#define DV_SPSR_I	0x00000080		/* IRQ mask - 1 to disable */
#define DV_SPSR_A	0x00000100		/* SError mask - 1 to disable */
#define DV_SPSR_D	0x00000200		/* Debug mask - 1 to disable */
#define DV_SPSR_IL	0x00100000		/* Illegal execution state */
#define DV_SPSR_SS	0x00200000		/* Software step */
#define DV_SPSR_PAN	0x00400000		/* Armv8.1 - privileged access never */
#define DV_SPSR_UAO	0x00800000		/* Armv8.3 - override unprivileged store */
#define DV_SPSR_V	0x10000000		/* Condition flags ... */
#define DV_SPSR_C	0x20000000
#define DV_SPSR_Z	0x40000000
#define DV_SPSR_N	0x80000000

#define DV_INTENABLED		0
#define DV_INTDISABLED		DV_SPSR_I

/* mrs and msr have to be macros because the register name is passed as a literla string
*/
#define dv_arm64_mrs(regname) \
({	dv_u64_t MRSresult;								\
	__asm__ volatile ("mrs %[result], " #regname	\
		: [result] "=r" (MRSresult)					\
		: /* no inputs */							\
		: /* nothing clobbered */);					\
	MRSresult;										\
})

#define dv_arm64_msr(regname, value) \
({	__asm__ volatile ("msr " #regname ", %[val]"	\
		: /* no outputs */							\
		: [val] "r" (value)							\
		: /* nothing clobbered */);					\
})

#if !DV_ASM

static inline dv_u64_t dv_get_SP(void)
{
	dv_u64_t x;
	__asm__ volatile ("mov  %0, sp" : "=r"(x) : );
	return x;
}

static inline void dv_set_vbar_el3(dv_u64_t vbar)
{
	dv_arm64_msr(VBAR_EL3, vbar);
	__asm__ volatile ("isb	sy");
}

static inline void dv_set_vbar_el2(dv_u64_t vbar)
{
	dv_arm64_msr(VBAR_EL2, vbar);
	__asm__ volatile ("isb	sy");
}

static inline void dv_set_vbar_el1(dv_u64_t vbar)
{
	dv_arm64_msr(VBAR_EL1, vbar);
	__asm__ volatile ("isb	sy");
}

static inline dv_u32_t dv_get_el(void)
{
	return (dv_arm64_mrs(CurrentEL) >> 2) & 3;
}

void dv_init_core(void);
void dv_switch_el2el1(void);
void dv_switch_el3el1(void);
void dv_switch_el3el2(void);

#endif

#endif
