/*	dv-memset32.c - out-of-line version of dv_memset32
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
#include <dv-string.h>

/* dv_memset32() - initialise an array.
 *
 * dv_memset32(unsigned *d, unsigned v, unsigned n) initialises every element of
 * the array d to the value v. n is the number of elements.
*/
unsigned *dv_memset32(unsigned *d, unsigned v, unsigned n)
{
	return dv_memset32_inline(d, v, n);
}
