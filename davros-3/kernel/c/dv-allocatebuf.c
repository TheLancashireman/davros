/*	dv-allocatebuf.c - buffer allocator for davros
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
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-coreconfig.h>

DV_COVDEF(allocatebuf);

/* dv_allocate_buf() - allocate a portion of the ringbuffer array
*/
dv_index_t dv_allocate_buf(dv_kobjallocator_t *allocator, dv_quantity_t n_req)
{
	dv_index_t i;

	DV_DBG(dv_kprintf("dv_allocate_buf(0x%x, %d, %d): n_free = %d\n", allocator, n_req, allocator->n_free));

	if ( allocator->n_free < n_req )
	{
		DV_DBG(dv_kprintf("dv_allocate_obj() failed because allocator->n_free < n_req\n"));
		return -1;
	}

	i = allocator->next;
	allocator->n_free -= n_req;
	allocator->next += n_req;

	return i;
}

/* dv_rb_allocate() - allocate the buffer array for a ringbuffer
*/
void dv_rb_allocate(dv_kernel_t *kvars, dv_ringbuffer_t *rb)
{
	dv_index_t i;

	/* Number of buffer elements to allocate.
	*/
	rb->n_alloc = (rb->size * rb->length + sizeof(*rb->buf) - 1)/sizeof(*rb->buf);

	i = dv_allocate_buf(&kvars->rbbuf_allocator, rb->n_alloc);

	if ( i < 0 )
	{
		rb->n_alloc = 0;
		dv_panic(dv_panic_initialisationerror, "dv_rb_allocate", "insufficient buffer space");
	}
	else
	{
		rb->buf = &dv_coreconfigs[kvars->core_index]->buffers[i];
	}
}
