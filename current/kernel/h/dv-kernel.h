/*	dv-kernel.h - kernel variables for davros
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
#ifndef dv_kernel_h
#define dv_kernel_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

#if !DV_ASM

struct dv_kernel_s
{
	dv_thread_t *current_thread;
	dv_stackword_t *kernel_sp;
	dv_identity_t core_index;
	dv_doublylinkedlist_t thread_queue;
	dv_panic_t panic_reason[2];
};

static inline dv_thread_t *dv_threadqueuehead(dv_kernel_t *kvars)
{
	return (kvars->thread_queue.headtail.successor->payload);
}

void dv_start(dv_identity_t);

extern dv_kernel_t *const dv_kernelvars[DV_NCORES];
extern dv_stackword_t *const dv_kernelstack[DV_NCORES];
extern dv_stackword_t *const dv_kernelstacktop[DV_NCORES];

#endif

#define DV_OFFSET_kvars_current_thread		0
#define DV_OFFSET_kvars_kernelsp			DV_SIZE_PTR

/* DV_CORELIST() creates a list of initialisers for an array with one entry per core.
 *
 * Currently up to 8 cores are supported.
*/
#if DV_NCORES == 1
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0
#elif DV_NCORES == 2
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1
#elif DV_NCORES == 3
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2
#elif DV_NCORES == 4
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3
#elif DV_NCORES == 5
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4
#elif DV_NCORES == 6
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5
#elif DV_NCORES == 7
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5, c6
#elif DV_NCORES == 8
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5, c6, c7
#endif

#endif
