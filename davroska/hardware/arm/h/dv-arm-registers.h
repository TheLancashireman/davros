/*	dv-arm-registers.h - ARM registers for davroska
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
#ifndef dv_arm_registers_h
#define dv_arm_registers_h	1

#define DV_ARM_MODE			0x1f	/* Mask for mode */
#define DV_ARM_MODE_USR		0x10
#define DV_ARM_MODE_FIQ		0x11
#define DV_ARM_MODE_IRQ		0x12
#define DV_ARM_MODE_SVC		0x13
#define DV_ARM_MODE_ABT		0x17
#define DV_ARM_MODE_UNDEF	0x1b
#define DV_ARM_MODE_SYS		0x1f	/* available on ARM Arch 4 and later */

#define DV_ARM_IRQ_DIS		0x80	/* when I bit is set, IRQ is disabled */
#define DV_ARM_FIQ_DIS		0x40	/* when F bit is set, FIQ is disabled */

#define DV_INTENABLED		0
#define DV_INTDISABLED		DV_ARM_IRQ_DIS

/* mrs and msr have to be macros because the register name is passed as a literal string
*/
#define dv_arm_mrs(regname) \
({	dv_u32_t MRSresult;								\
	__asm__ volatile ("mrs %[result], " #regname	\
		: [result] "=r" (MRSresult)					\
		: /* no inputs */							\
		: /* nothing clobbered */);					\
	MRSresult;										\
})

#define dv_arm_msr(regname, value) \
({	__asm__ volatile ("msr " #regname ", %[val]"	\
		: /* no outputs */							\
		: [val] "r" (value)							\
		: /* nothing clobbered */);					\
})

#if !DV_ASM
static inline dv_u32_t dv_arm_get_SP(void)
{
	dv_u32_t x;
	__asm__ volatile ("mov  %0, sp" : "=r"(x) : );
	return x;
}
#endif

#endif
