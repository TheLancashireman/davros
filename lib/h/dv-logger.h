/*	dv-logger.h - an abstract data logger
 *
 *	Copyright David Haworth
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
#ifndef dv_logger_h
#define dv_logger_h	1

/* How it works:
 *
 *	- Management of the actual buffer is up to the caller.
 *	- If the read and write indexes are the same, the buffer is empty.
 *	- If incrementing the write index would cause it to be the same as the read index, the buffer is full.
 *	- Implication: there is always an unused element of the buffer, so if you want to be able to log n items,
 *	  your buffer length must be n+1
 *	- Advantages: simple code, no thread contention over structure
 *	- Disadvantage: wasted space
 *
 * It's possible to avoid wasting space by using a 'wrap flag', but that complicates the code somewhat.
 *
 * How to use:
 *
 * Initialise the logger structure
 *
 * To write:
 *	- get the write index
 *	- copy data to the buffer element indicated by the index
 *	- update the write index
 *
 * To read:
 *	- get the read index
 *	- copy data from the buffer element indicated by the index
 *	- update the read index
 *
 * If your hardware or compiler has a habit of re-ordering loads and stores, put memory barriers after the
 * copy and after updating the index.
 * On a multi-core instance a cache that isn't coherent these functions won't work. You'll need to ensure
 * that the indexes are in different cache lines. For interference protection between reader and writer
 * the indexes need to be in separate regions.
*/

typedef struct dv_logger_s
{
	dv_i16_t length;				/* No. of elements in the buffer array. Constant. */
	volatile dv_i16_t readIdx;		/* Where the next element shall be read from. Updated by reader. */
	volatile dv_i16_t writeIdx;		/* Where the next element can be written to. Updated by writer. */
	dv_i16_t overflow;				/* Overflow flag. Updated by writer. */
} dv_logger_t;

/* dv_log_init() - initialise the structure
*/
static inline void dv_log_init(dv_logger_t *l, dv_i16_t len)
{
	l->length = len;
	l->readIdx = 0;
	l->writeIdx = 0;
	l->overflow = 0;
}

/* dv_log_next_idx() - returns next value of an index
*/
static inline dv_i16_t dv_log_next_idx(dv_logger_t *l, dv_i16_t i)
{
	dv_i16_t n = i + 1;
	if ( n >= l->length )
		return 0;
	return n;
}

/* dv_log_get_write_idx() - returns the index at which to write the next item to log.
 *
 * Returns -1 if buffer is full.
*/
static inline dv_i16_t dv_log_get_write_idx(dv_logger_t *l)
{
	dv_i16_t w = l->writeIdx;
	dv_i16_t n = dv_log_next_idx(l, w);
	if ( n == l->readIdx )
	{
		l->overflow = 1;
		return -1;
	}
	l->overflow = 0;
	return w;
}

/* dv_log_upd_write_idx() - increments the write index
*/
static inline dv_i16_t dv_log_upd_write_idx(dv_logger_t *l)
{
	return l->writeIdx = dv_log_next_idx(l, l->writeIdx);
}

/* dv_log_get_read_idx() - returns the index from which to read the oldest item logged
 *
 * Returns -1 if buffer is empty.
*/
static inline dv_i16_t dv_log_get_read_idx(dv_logger_t *l)
{
	dv_i16_t r = l->readIdx;
	if ( r == l->writeIdx )
		return -1;
	return r;
}

/* dv_log_upd_read_idx() - increments the read index
*/
static inline dv_i16_t dv_log_upd_read_idx(dv_logger_t *l)
{
	return l->readIdx = dv_log_next_idx(l, l->readIdx);
}

#endif
