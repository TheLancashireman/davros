/*	dv-arm-kconfig.h - kernel configuration for ARM processors
 *
 *	Copyright 2015 David Haworth
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
#ifndef dv_arm_kconfig_h
#define dv_arm_kconfig_h	1

/* 32-bit processor with no oddities.
*/
#include <kernel/h/dv-types-32.h>

/* Size of kernel stack in stackwords.
*/
#define DV_KSTACK_WORDS		200

/* Parameters for memory regions, including stacks.
 *
 * DV_MEM_PAGESIZE - granularity of memory management blocks.
 * DV_MEM_GAP - minimum gap to leave at bottom of block.
*/
#define DV_MEM_PAGESIZE	4096	/* ARM MMU page size */
#define DV_MEM_GAP		0		/* ARM MMU doesn't allow multi-byte accesses to spill */

/* Include files for ARM.
*/
#define DV_REGISTERS			<cpufamily/arm/h/dv-arm-registers.h>
#define DV_DISPATCH				<cpufamily/arm/h/dv-arm-dispatch.h>

/* Vector table location. For ARM processors that have RAM at address 0 we copy the vectors
 * there.
 * If there's read-only memory at 0 we have to flash a vector table that redirects to somewhere in
 * RAM, or maybe there's a vector base address register.
*/
#define DV_VECTOR_LOCATION		0

#if !DV_ASM

static inline void dv_hw_wait(void)
{
	__asm__ volatile ("wfi");
}

#endif

#endif
