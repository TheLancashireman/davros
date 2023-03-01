/*	dv-ringbuf.h
 *
 *	Copyright David Haworth
 *
 *	This file is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2, or (at your option)
 *	any later version.
 *
 *	It is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; see the file COPYING.  If not, write to
 *	the Free Software Foundation, 59 Temple Place - Suite 330,
 *	Boston, MA 02111-1307, USA.
 *
 *	This file contains some ringbuffer management functions.
 *
 *	A ringbuffer is an array of elements along with some management data.
 *	This file provides the management data types and the management functions.
 * 	The functions deal only with the managment of the current positions, which are indexes into
 *	the buffer.
 *	This means that this file can be used for ringbuffers with elements of any data type.
 *	Functions are provided here to manage ringbuffers of characters.
*/

#ifndef dv_ringbuf_h
#define dv_ringbuf_h	1

typedef struct dv_rbm_s dv_rbm_t;

struct dv_rbm_s
{
	volatile dv_i32_t head;		/* The index into which the next "write" will occur */
	volatile dv_i32_t tail;		/* The index from which the next "read" will occur */
	dv_i32_t length;			/* The length of the associated buffer */
};

/* Rules:
 *	elements are removed from the head and added at the tail.
 *	head == tail  =>  buffer is empty
 *  (tail+1) mod length == head  => buffer is full
*/

/* dv_rb_add1() - add 1 to a given index, wrapping at length.
 *
 * Assumption: i is in range 0..length-1. If >= length result is 0.
*/
static inline dv_i32_t dv_rb_add1(dv_rbm_t *rb, dv_i32_t i)
{
	if ( (i+1) >= rb->length ) return 0;
	return i+1;
}

/*  dv_rb_empty() - returns true if rb is empty (head == tail), false otherwise
*/
static inline dv_boolean_t dv_rb_empty(dv_rbm_t *rb)
{
	if ( rb->head == rb->tail )
		return 1;
	return 0;
}

/*  dv_rb_full() - returns true if rb is full, false otherwise
*/
static inline dv_boolean_t dv_rb_full(dv_rbm_t *rb)
{
	if ( dv_rb_add1(rb, rb->tail) == rb->head )
		return 1;
	return 0;
}

/*	dv_rb_u8_put() - append a byte to a ringbuffer and return 1 if successful.
 *
 *	If the buffer is full the character is discarded and dv_rb_u8_put() returns 0
*/
static inline dv_boolean_t dv_rb_u8_put(dv_rbm_t *rbm, dv_u8_t buf[], dv_u8_t c)
{
	if ( dv_rb_full(rbm) )
		return 0;
	buf[rbm->tail] = c;
	rbm->tail = dv_rb_add1(rbm, rbm->tail);
	return 1;
}

/*	dv_rb_u8_get() - append a byte to a ringbuffer and return 1 if successful.
 *
 *	If the buffer is empty the dv_rb_u8_get() returns -1
*/
static inline int dv_rb_u8_get(dv_rbm_t *rbm, dv_u8_t buf[])
{
	if ( dv_rb_empty(rbm) )
		return -1;
	int c = (int)buf[rbm->head];
	rbm->head = dv_rb_add1(rbm, rbm->head);
	return c;
}

#endif
