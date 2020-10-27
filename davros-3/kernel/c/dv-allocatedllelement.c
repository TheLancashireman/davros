/*	dv-allocatedllelement.c - allocate a dll_element for an executable
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-doublylinkedlist.h>

DV_COVDEF(allocatedllelement);

/* dv_is_free_dllelement() - return true if dll_element i is free
*/
static dv_boolean_t dv_is_free_dllelement(dv_index_t i, const void *dllelem)
{
	const dv_dllelement_t *dllelem_tbl = dllelem;
	return dllelem_tbl[i].payload_type == dv_dll_free;
}

/* dv_allocate_dllelement() - allocate a dllelement structure
 *
 * Find and allocate a free dllelement structure for an executable.
 *
 * No sharing, so this is a simple allocator.
*/
dv_dllelement_t *dv_allocate_dllelement(dv_kernel_t *kvars)
{
	dv_dllelement_t *dllelem_tbl;
	int dllelem_i;
	int n_dllelem;

	dllelem_tbl = dv_coreconfigs[kvars->core_index]->dll_elements;
	n_dllelem = dv_coreconfigs[kvars->core_index]->n_dll_elements;
	dllelem_i = dv_allocate_obj(&kvars->dllelem_allocator, n_dllelem, dv_is_free_dllelement, dllelem_tbl);

	if ( dllelem_i < 0 )
		return DV_NULL;

	dllelem_tbl[dllelem_i].payload_type = dv_dll_exe;

	return &dllelem_tbl[dllelem_i];
}

/* dv_deallocate_dllelement() - deallocate a dllelement structure
 *
 * No sharing, so this is a simple deallocator.
*/
void dv_deallocate_dllelement(dv_kernel_t *kvars, dv_dllelement_t *elem)
{
	elem->payload_type = dv_dll_free;
	dv_deallocate_obj(&kvars->dllelem_allocator);
}
