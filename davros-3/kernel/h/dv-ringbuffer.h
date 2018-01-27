/*	dv-ringbuffer.h - a ringbuffer for davros
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
#ifndef dv_ringbuffer_h
#define dv_ringbuffer_h		1

#include <kernel/h/dv-kernel-types.h>

#if !DV_ASM

/* dv_ringbuffer_t - a ringbuffer descriptor
 *
 * Each ringbuffer has:
 *	- readerState. Contains the index of the next element to read.
 *	- writerState. Contains the index of the next element to write.
 *	- length. This is the number of elements in the ringbuffer.
 *	- size. Size in bytes of each element. Either 1, 2 or 4.
 *	- buf. The actual ring buffer. An array of (size+3)/4 32-bit words.
 *
 * If the reader's index and the writer's index are the same, the buffer is empty.
 * If the (reader's index + 1) mod length and the writer's index are the same, the buffer is full.
 *
 * If the ring buffer is a "local" buffer (e.g. same core using interrupt lock) the reader state is
 * in the structure itself.
 * If the ring buffer is a "lockfree" buffer (e.g. inter-core message queue) the reader state is remote
 * and the structure contains a pointer to it.
 *
 * In the lockfree case:
 * 	- The writer needs write access to the buffer and the writer state, and only read access to the reader state.
 * 	- The reader needs write access to the reader state and only read access to the buffer and the writer state.
 *
 * Local ringbuffers are used for job queues and for purposes like serial i/o buffers that are maneged on a
 * single core or in shared memory with suitable mutual exclusion.
 * Lockfree ringbuffers are used for purposes like inter-core message queues. If there are multiple
 * writers, the buffer structure is in shared memory with suitable mutex for writers. The (single) reader is lock-free.
*/
enum dv_rbtype_e
{
	rb_simple = 1,		/* Simple queue - bastic types (u8, u16, u32) are added individually. */
	rb_complex,			/* Complex queue - variable-length entries with metadata */
	rb_msgqueue			/* Message queue (complex, lock-free, protected) */
};

typedef enum dv_rbtype_e dv_rbtype_t;

typedef struct dv_rbstate_s dv_rbstate_t;

struct dv_rbstate_s
{
	dv_quantity_t index;
};

typedef dv_rbstate_t dv_rbwriterstate_t;
typedef dv_rbstate_t dv_rbreaderstate_t;

struct dv_ringbuffer_s
{
	dv_rbtype_t rbType;					/* What kind of ring buffer is this? */
	dv_rbwriterstate_t writerState;
	union
	{
		dv_rbreaderstate_t *readerStateRef;
		dv_rbreaderstate_t readerState;
	} readerState_u;
	dv_quantity_t length;				/* Number of objects in the buffer */
	dv_quantity_t size;					/* Size of each object in the buffer (in bytes) */
	dv_u32_t *buf;						/* The buffer - contains at least (length*size) bytes */
};

dv_quantity_t dv_rb_remove_simple(dv_ringbuffer_t *, void *);
dv_quantity_t dv_rb_append_simple(dv_ringbuffer_t *, void *);

static inline dv_quantity_t dv_rb_increment(dv_quantity_t index, dv_quantity_t incr, dv_quantity_t length)
{
	dv_quantity_t i = index + incr;
	return (i < length) ? i : (i - length);
}

#endif

#endif
