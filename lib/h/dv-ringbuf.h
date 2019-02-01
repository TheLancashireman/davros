/*	dv-ringbuf.h
 *
 *	Copyright 2019 David Haworth
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
*/

#ifndef dv_ringbuf_h
#define dv_ringbuf_h	1

typedef struct dv_rbm_s dv_rbm_t;

struct dv_rbm_s
{
	dv_i32_t head;		/* The index into which the next "write" will occur */
	dv_i32_t tail;		/* The index from which the next "read" will occur */
	dv_i32_t length;	/* The length of the associated buffer */
};

/* Rules:
 *	head == tail  =>  buffer is empty
 *  (head+1) mod length == tail  => buffer is full
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

/*  dv_rb_isempty() - returns true if rb is empty (head == tail), false otherwise
*/
static inline dv_boolean_t dv_rb_isempty(dv_rbm_t *rb)
{
	return ( rb->head == rb->tail );
}

/*  dv_rb_isfull() - returns true if rb is full, false otherwise
*/
static inline dv_boolean_t dv_rb_full(dv_rbm_t *rb)
{
	return ( dv_rb_add1(rb, rb->head) == rb->tail );
}

#endif
