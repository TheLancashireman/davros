/*	dv-arm64-core.h - ARM64 core utilities
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
#ifndef dv_arm64_core_h
#define dv_arm64_core_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

/* Moving data to and from CP15 registers.
 * On arm64 it's much easier because each register has a mnemonic that the assembler understands.
 *
 * These utilities have to be macros because the name of the register is passed to the MSR/MRS instruction
 * at compile time.
*/

#define dv_arm64_mrs(regname) \
({	u64 MRSresult;									\
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

static inline void dv_set_vbar_el1(dv_u64_t vbar) \
{
	dv_arm64_msr(VBAR_EL1, vbar);
	__asm__ volatile ("isb	sy");
}


#endif
