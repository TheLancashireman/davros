/*	dv-rbappendsimple.c - dv_rb_append_simple()
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
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-coverage.h>

DV_COVDEF(function);

/* dv_rb_append_simple() - append an element to a (local) ring buffer
 *
 * appends the given element to a ring buffer
 *
 * Return value:
 *	0 - the ringbuffer is full
 * >0 - the number of elements copied
*/
dv_quantity_t dv_rb_append_simple
(	dv_ringbuffer_t *rb,
	void *in
)
{
	dv_quantity_t new_index;

	if ( rb->buf == DV_NULL )
	{
		dv_panic(dv_panic_ringbuffernotallocated, "dv_rb_append_simple", "ringbuffer creation is incomplete");
	}

	if ( rb->rb_type != rb_simple )
	{
		dv_panic(dv_panic_ringbufferwrongtype, "dv_rb_append_simple", "ringbuffer is wrong type");
	}

	/* Calculate where the writer index would be if we did this.
	*/
	new_index = dv_rb_increment(rb->writer_state.index, 1, rb->length);

	if ( new_index == rb->reader_state_u.reader_state.index )
	{
		return 0;	/* Buffer is full */
	}

	switch ( rb->size )
	{
	case 1:
		((dv_u8_t *)(&rb->buf[0]))[rb->writer_state.index] = *(dv_u8_t *)in;
		break;
	case 2:
		((dv_u16_t *)(&rb->buf[0]))[rb->writer_state.index] = *(dv_u16_t *)in;
		break;
	case 4:
		rb->buf[rb->writer_state.index] = *(dv_u32_t *)in;
		break;
	default:
		dv_panic(dv_panic_ringbufferinvalidsize, "dv_rb_append_simple", "ringbuffer has invalid element size");
		break;
	}

	rb->writer_state.index = new_index;

	return 1;
}

/* man-page-generation - to be defined
*/
