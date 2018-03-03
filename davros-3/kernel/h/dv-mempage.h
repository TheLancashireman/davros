/*	dv-mempage.h - memory management for davros
 *
 *	Copyright 2017 David Haworth
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
#ifndef dv_mempage_h
#define dv_mempage_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>

/* This is a very simple way of allocating memory (e.g. for stacks)s, designed for use
 * with an MMU or an MPU with large fixed granularity.
 * A page of memory is represented by the dv_mempage_t structure. The system has an array
 * of these, correctly aligned on a page boundary.
 * We can't assume that the kernel has write access to the pages -- and in any case we don't
 * want to steal a word or two from each page for management -- so they are managed by
 * means of a parallel array of dv_mempagestatus_t structures.
 * cf. "slab allocator" in linux kernel 2.x
*/

struct dv_page_s
{
	dv_machineword_t words[DV_MEM_PAGESIZE/sizeof(dv_machineword_t)];
};

struct dv_mempage_s
{
	dv_page_t *page;
	int n_use;
};

dv_mempage_t *dv_allocate_page(dv_kernel_t *);

#endif
