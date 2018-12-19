/*	dv-allocateeventstatus.c - allocate an eventstatus for an executable
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
#include <kernel/h/dv-event.h>

DV_COVDEF(allocateeventstatus);

/* dv_is_free_eventstatus() - return true if eventstatus i is free
*/
static dv_boolean_t dv_is_free_eventstatus(dv_index_t i, const void *evs)
{
	const dv_eventstatus_t *evs_tbl = evs;
	return !evs_tbl[i].allocated;
}

/* dv_allocate_eventstatus() - allocate an eventstatus structure
 *
 * Find and allocate a free eventstatus structure for an executable.
 *
 * No sharing, so this is a simple allocator.
*/
dv_eventstatus_t *dv_allocate_eventstatus(dv_kernel_t *kvars)
{
	dv_eventstatus_t *evs_tbl;
	int evs_i;
	int n_evs;

	evs_tbl = dv_coreconfigs[kvars->core_index]->events;
	n_evs = dv_coreconfigs[kvars->core_index]->n_events;
	evs_i = dv_allocate_obj(&kvars->evs_allocator, n_evs, dv_is_free_eventstatus, evs_tbl);

	if ( evs_i < 0 )
		return DV_NULL;

	evs_tbl[evs_i].allocated = 1;

	return &evs_tbl[evs_i];
}
