/*	dv-rbremovesimple.c - dv_rb_remove_simple()
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

DV_COVDEF(function);

/* dv_rb_remove_simple() - remove an element from a (simple local) ring buffer
 *
 * Removes the next element from a ring buffer and places it in the space provided.
 *
 * Return value:
 *	0 - the ringbuffer is empty
 * >0 - the number of elements copied
*/
dv_quantity_t dv_rb_remove_simple
(	dv_ringbuffer_t *rb,
	void *out
)
{
	if ( rb->buf == DV_NULL )
	{
		dv_panic(dv_panic_ringbuffernotallocated, "dv_rb_remove_simple", "ringbuffer creation is incomplete");
	}

	if ( rb->rb_type != rb_simple )
	{
		dv_panic(dv_panic_ringbufferwrongtype, "dv_rb_remove_simple", "ringbuffer is wrong type");
	}

	if ( rb->reader_state_u.reader_state.index == rb->writer_state.index )
	{
		return 0;	/* Buffer is empty */
	}

	switch ( rb->size )
	{
	case 1:
		*(dv_u8_t *)out = ((dv_u8_t *)(&rb->buf[0]))[rb->reader_state_u.reader_state.index];
		break;
	case 2:
		*(dv_u16_t *)out = ((dv_u16_t *)(&rb->buf[0]))[rb->reader_state_u.reader_state.index];
		break;
	case 4:
		*(dv_u32_t *)out = rb->buf[rb->reader_state_u.reader_state.index];
		break;
	default:
		dv_panic(dv_panic_ringbufferinvalidsize, "dv_rb_remove_simple", "ringbuffer has invalid element size");
		break;
	}

	rb->reader_state_u.reader_state.index = dv_rb_increment(rb->reader_state_u.reader_state.index, 1, rb->length);

	return 1;
}

/* man-page-generation - to be defined
*/
