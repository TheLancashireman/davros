/*	dv-allocateringbuffer.c - allocate a ringbuffer
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
#include <kernel/h/dv-ringbuffer.h>

DV_COVDEF(allocateringbuffer);

/* dv_is_free_ringbuffer() - return true if ringbuffer i is free
*/
static dv_boolean_t dv_is_free_ringbuffer(dv_index_t i, const void *tbl)
{
	const dv_ringbuffer_t *rb_tbl = tbl;
	if ( rb_tbl[i].rb_type == rb_unused )
		return 1;
	return 0;
}

/* dv_allocate_ringbuffer() - allocate a ringbuffer
 *
 * Find and allocate a free ringbuffer.
 *
 * No sharing, so this is a simple allocator.
*/
dv_ringbuffer_t *dv_allocate_ringbuffer(dv_kernel_t *kvars)
{
	dv_ringbuffer_t *rb_tbl;
	int rb_i;
	int n_rb;

	rb_tbl = dv_coreconfigs[kvars->core_index]->ringbuffers;
	n_rb = dv_coreconfigs[kvars->core_index]->n_ringbuffers;
	rb_i = dv_allocate_obj(&kvars->rb_allocator, n_rb, dv_is_free_ringbuffer, rb_tbl);

	if ( rb_i < 0 )
		return DV_NULL;

	rb_tbl[rb_i].rb_type = rb_reserved;

	return &rb_tbl[rb_i];
}

/* man-page-generation - to be defined
*/
