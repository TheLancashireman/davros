/*	dv-allocatepage.c - allocate a page of memory
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-mempage.h>
#include <lib/h/dv-string.h>

DV_COVDEF(allocatepage);

/* dv_is_free_page() - return true if page i is free
*/
static dv_boolean_t dv_is_free_page(dv_index_t i, const void *tbl)
{
	const dv_mempage_t *pg_tbl = tbl;
	if ( pg_tbl[i].n_use == 0 )
		return 1;
	return 0;
}

/* dv_allocate_page() - allocate a page of memory
 *
 * Find and allocate a page of memory
*/
dv_mempage_t *dv_allocate_page(dv_kernel_t *kvars)
{
	dv_mempage_t *pge_tbl = dv_coreconfigs[kvars->core_index]->mempage;
	int n_pge = dv_coreconfigs[kvars->core_index]->n_pages;
	int pge_i = dv_allocate_obj(&kvars->page_allocator, n_pge, dv_is_free_page, pge_tbl);

	if ( pge_i < 0 )
		return DV_NULL;

	pge_tbl[pge_i].n_use = 1;
	pge_tbl[pge_i].page = dv_memset32(&dv_coreconfigs[kvars->core_index]->pages[pge_i], 0, DV_MEM_PAGESIZE);
	

	return &pge_tbl[pge_i];
}
