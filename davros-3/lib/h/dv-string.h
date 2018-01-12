/*	dv-string.h - assorted string functions for davros
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
#ifndef dv_string_h
#define dv_string_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

/* dv_memset32() - initialise an array.
 *
 * dv_memset32(dv_u32_t *d, dv_u32_t v, size_t n) initialises every element of
 * the array d to the value v. n is the number of elements.
*/
static inline dv_u32_t *dv_memset32(dv_u32_t *d, dv_u32_t v, dv_size_t n)
{
	while ( n > 0 )
	{
		n--;
		d[n] = v;
	}

	return (dv_u32_t *)d;
}

#endif
