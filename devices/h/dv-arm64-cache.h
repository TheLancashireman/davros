/*	dv-arm64-cache.h - arm64 cache control via DC/IC instructions
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
#ifndef dv_arm64_cache_h
#define dv_arm64_cache_h	1

#include <dv-devices.h>

/* aarch64 uses two instructions for cache meaintenance :  IC for the instruction cache and DC for the data cache.
 *
 *	For the instruction cache:
 *	IC IALLUIS  Invalidate all to Point of Unification, Inner Shareable
 *	IC IALLU    Invalidate all to Point of Unification
 *	IC IVAU, Xt Invalidate by virtual address to Point of Unification
 *
 *	For the data cache:
 *	DC IVAC, Xt  Invalidate by virtual address to Point of Coherency
 *	DC ISW, Xt   Invalidate by set/way
 *	DC CVAC, Xt  Clean by virtual address to Point of Coherency
 *	DC CVAP, Xt  Clean by virtual address to Point of Persistence (only for ARMv8.2-DCPoP)
 *	DC CSW, Xt   Clean by set/way
 *	DC CVAU, Xt  Clean by virtual address to Point of Unification
 *	DC CIVAC, Xt Clean and invalidate by virtual address to Point of Coherency
 *	DC CISW, Xt  Clean and invalidate by set/way
 *
 * To invalidate or flush the entire cache, we'll have to loop over all the sets and ways
*/

/* Invalidate instruction cache
*/
static inline void dv_invalidate_entire_instruction_cache(void)
{
	__asm__ volatile ("ic  IALLU" : : : );
}

static inline void dv_invalidate_entire_instruction_cache_inner_shareable(void)
{
	__asm__ volatile ("ic  IALLUIS" : : : );
}

static inline void dv_invalidate_instruction_cache_by_mva(dv_u64_t mva)
{
	__asm__ volatile ("ic  IVAU, %0" : : "r"(mva) : );
}

static inline void dv_flush_prefetch_buffer(void)
{
	/* TODO */
}

static inline void dv_flush_entire_branch_target_cache(void)
{
	/* TODO */
}

static inline void dv_flush_branch_target_cache_by_mva(dv_u64_t mva)
{
	/* TODO */
}


/* Invalidate data cache
*/
static inline void dv_invalidate_entire_data_cache(void)
{
	/* TODO */
}

static inline void dv_invalidate_data_cache_by_mva(dv_u64_t mva)
{
	__asm__ volatile ("dc  IVAC, %0" : : "r"(mva) : );
}

static inline void dv_invalidate_data_cache_by_index(dv_u64_t index)
{
	__asm__ volatile ("dc  ISW, %0" : : "r"(mva) : );
}


/* Invalidate both caches
*/
static inline void dv_invalidate_both_caches(void)
{
	dv_invalidate_entire_instruction_cache();
	dv_invalidate_entire_data_cache();
}


/* Clean data cache (c7/c10)
*/
static inline void dv_clean_entire_data_cache(void)
{
	/* TODO */
}

static inline void dv_clean_data_cache_by_mva_to_PoC(dv_u64_t mva)
{
	__asm__ volatile ("dc  CVAC, %0" : : "r"(mva) : );
}

static inline void dv_clean_data_cache_by_mva_to_PoP(dv_u64_t mva)	/* ARMv8.2-DCPoP only */
{
	__asm__ volatile ("dc  CVAC, %0" : : "r"(mva) : );
}

static inline void dv_clean_data_cache_by_mva_to_PoU(dv_u64_t mva)
{
	__asm__ volatile ("dc  CVAU, %0" : : "r"(mva) : );
}

static inline void dv_clean_data_cache_by_mva(dv_u64_t mva)
{
	dv_clean_data_cache_by_mva_to_PoC(mva);
}

static inline void dv_clean_data_cache_by_index(dv_u64_t index)
{
	__asm__ volatile ("dc  CSW %0" : : "r"(mva) : );
}

static inline void dv_synchronization_barrier(void)
{
	/* TODO */
}

static inline void dv_memory_barrier(void)
{
	/* TODO */
}


/* Clean and invalidate data cache (c7/c14)
*/
static inline void dv_clean_and_invalidate_entire_data_cache(void)
{
	/* TODO */
}

static inline void dv_clean_and_invalidate_data_cache_by_mva(dv_u64_t mva)
{
	__asm__ volatile ("dc  CIVAC, %0" : : "r"(mva) : );
}

static inline void dv_clean_and_invalidate_data_cache_by_index(dv_u64_t index)
{
	__asm__ volatile ("dc  CISW, %0" : : "r"(mva) : );
}

/* Cache operations by range
*/
static inline void dv_invalidate_instruction_cache_range(dv_u64_t start, dv_u64_t end)
{
	/* TODO */
}

static inline void dv_invalidate_data_cache_range(dv_u64_t start, dv_u64_t end)
{
	/* TODO */
}

static inline void dv_clean_data_cache_range(dv_u64_t start, dv_u64_t end)
{
	/* TODO */
}

static inline void dv_clean_and_invalidate_data_cache_range(dv_u64_t start, dv_u64_t end)
{
	/* TODO */
}

#endif
