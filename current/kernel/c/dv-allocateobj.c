/*	dv-allocateobj.c - object allocator for davros
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-kernel.h>

DV_COVDEF(allocate_obj);

/* dv_allocate_obj() - allocate an unused object in an array
*/
dv_index_t dv_allocate_obj
(	dv_kobjallocator_t *allocator,
	dv_quantity_t n_obj,
	dv_boolean_t (*is_free)(dv_index_t, const void *),
	const void *tbl
)
{
	int i;
	dv_index_t obj_i = allocator->next;
	dv_index_t tmp;

	if ( allocator->n_free <= 0 )
		return -1;

	for ( i = 0; i < n_obj; i++ )
	{
		tmp = obj_i;
		obj_i++;
		if ( obj_i >= n_obj )
			obj_i = 0;

		if ( (*is_free)(tmp, tbl) )
		{
			allocator->n_free--;
			allocator->next = obj_i;
			return tmp;
		}
	}

	return -1;
}

/* man-page-generation - to be defined
*/