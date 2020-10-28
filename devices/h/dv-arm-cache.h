/*	dv-arm-cache.h - arm cache control via cp15
 *
 *	Copyright David Haworth
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
#ifndef dv_arm_cache_h
#define dv_arm_cache_h	1

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

/* Invalidate instruction cache (c7/c5)
*/
static inline void dv_invalidate_entire_instruction_cache(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 0" : : "r"(x) : );
}

static inline void dv_invalidate_instruction_cache_by_mva(dv_u32_t mva)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 1" : : "r"(mva) : );
}

static inline void dv_invalidate_instruction_cache_by_index(dv_u32_t index)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 2" : : "r"(index) : );
}

static inline void dv_flush_prefetch_buffer(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 4" : : "r"(x) : );
}

static inline void dv_flush_entire_branch_target_cache(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 6" : : "r"(x) : );
}

static inline void dv_flush_branch_target_cache_by_mva(dv_u32_t mva)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c5, 7" : : "r"(mva) : );
}


/* Invalidate data cache (c7/c6)
*/
static inline void dv_invalidate_entire_data_cache(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c6, 0" : : "r"(x) : );
}

static inline void dv_invalidate_data_cache_by_mva(dv_u32_t mva)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c6, 1" : : "r"(mva) : );
}

static inline void dv_invalidate_data_cache_by_index(dv_u32_t index)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c6, 2" : : "r"(index) : );
}


/* Invalidate both caches (c7/c7)
*/
static inline void dv_invalidate_both_caches(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c7, 0" : : "r"(x) : );
}


/* Clean data cache (c7/c10)
*/
static inline void dv_clean_entire_data_cache(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c10, 0" : : "r"(x) : );
}

static inline void dv_clean_data_cache_by_mva(dv_u32_t mva)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c10, 1" : : "r"(mva) : );
}

static inline void dv_clean_data_cache_by_index(dv_u32_t index)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c10, 2" : : "r"(index) : );
}

static inline void dv_synchronization_barrier(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c10, 4" : : "r"(x) : );
}

static inline void dv_memory_barrier(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c10, 5" : : "r"(x) : );
}


/* Clean and invalidate data cache (c7/c14)
*/
static inline void dv_clean_and_invalidate_entire_data_cache(void)
{
	dv_u32_t x = 0;
	__asm__ volatile ("mcr  p15, 0, %0, c7, c14, 0" : : "r"(x) : );
}

static inline void dv_clean_and_invalidate_data_cache_by_mva(dv_u32_t mva)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c14, 1" : : "r"(mva) : );
}

static inline void dv_clean_and_invalidate_data_cache_by_index(dv_u32_t index)
{
	__asm__ volatile ("mcr  p15, 0, %0, c7, c14, 2" : : "r"(index) : );
}

/* Cache operations by range
*/
static inline void dv_invalidate_instruction_cache_range(dv_u32_t start, dv_u32_t end)
{
	__asm__ volatile ("mcrr  p15, 0, %0, %0, c5" : : "r"(end), "r"(start) : );
}

static inline void dv_invalidate_data_cache_range(dv_u32_t start, dv_u32_t end)
{
	__asm__ volatile ("mcrr  p15, 0, %0, %0, c6" : : "r"(end), "r"(start) : );
}

static inline void dv_clean_data_cache_range(dv_u32_t start, dv_u32_t end)
{
	__asm__ volatile ("mcrr  p15, 0, %0, %0, c12" : : "r"(end), "r"(start) : );
}

static inline void dv_clean_and_invalidate_data_cache_range(dv_u32_t start, dv_u32_t end)
{
	__asm__ volatile ("mcrr  p15, 0, %0, %0, c14" : : "r"(end), "r"(start) : );
}

#endif
