/*	dv-kernel.h - kernel variables for davros
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
#ifndef dv_kernel_h
#define dv_kernel_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

#if !DV_ASM

/* struct dv_kobjallocator_s
 *
 * Used for allocating kernel objects during configuration - e.g. executables, locks etc.
 *
 * If n_free is 0 there are none left --> error.
 * If n_free is non-zero, start the search at index "next". When the index reaches "n", go back to 0.
 * When searching use a for loop ( i=0; i<n; i++ ). If the loop completes without finding a slot --> panic.
 * If free object found, decrement n_free and set next to the object after the one found (modulo n).
 * When an object is freed, increment n_free and (optionally) if the object at index "next" is in use, set next
 * to the index of the freed object.
*/
struct dv_kobjallocator_s
{
	dv_quantity_t n_free;			/* No. of free objects remaining */
	dv_index_t next;				/* Next object to begin search for a free object */
};

struct dv_kernel_s
{
	dv_thread_t *current_thread;
	dv_stackword_t *kernel_sp;
	dv_index_t core_index;
	dv_doublylinkedlist_t thread_queue;
	dv_doublylinkedlist_t sleep_queue;
	dv_panic_t panic_reason[2];
	dv_kobjallocator_t exe_allocator;
	dv_kobjallocator_t thr_allocator;
	dv_kobjallocator_t reg_allocator;
	dv_kobjallocator_t evs_allocator;
	dv_kobjallocator_t rb_allocator;
	dv_kobjallocator_t rbbuf_allocator;
	dv_kobjallocator_t dllelem_allocator;
	dv_kobjallocator_t page_allocator;
#if DV_CPU_HAS_KVARS
	DV_CPU_KVARS_TYPE cpu;
#endif
};

extern void dv_set_kvars(dv_kernel_t *kvars);
extern dv_kernel_t *dv_get_kvars(void);
extern void dv_init_vectors(void);

static inline dv_thread_t *dv_threadqueuehead(dv_kernel_t *kvars)
{
	return (kvars->thread_queue.headtail.successor->payload);
}

static inline void dv_deallocate_obj(dv_kobjallocator_t *allocator)
{
	allocator->n_free++;
}

static inline void dv_deallocate_stack(dv_kernel_t *kvars, dv_mempage_t *stackpage)
{
}

extern void dv_start(dv_index_t) __attribute__((noreturn));
extern void dv_init_kvars(dv_kernel_t *, const dv_coreconfig_t *);
extern void dv_dispatch(dv_kernel_t *kvars) __attribute__((noreturn));

extern dv_index_t dv_allocate_obj(dv_kobjallocator_t *, dv_quantity_t,
												dv_boolean_t (*is_free)(dv_index_t, const void *), const void *);

extern dv_thread_t *dv_allocate_thread(dv_kernel_t *, const dv_executable_t *);
extern void dv_deallocate_thread(dv_kernel_t *, dv_thread_t *);

extern dv_registers_t *dv_allocate_registers(dv_kernel_t *, const dv_executable_t *);
extern void dv_deallocate_registers(dv_kernel_t *, dv_registers_t *);

extern dv_mempage_t *dv_allocate_stack(dv_kernel_t *, const dv_executable_t *);

#endif

#define DV_OFFSET_kvars_current_thread		0
#define DV_OFFSET_kvars_kernelsp			DV_SIZE_PTR

#endif
