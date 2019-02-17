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

/* dv_memset32() - initialise an array.
 *
 * dv_memset32(unsigned *d, unsigned v, unsigned n) initialises every element of
 * the array d to the value v. n is the number of elements.
*/
static inline unsigned *dv_memset32_inline(unsigned *d, unsigned v, unsigned n)
{
	while ( n > 0 )
	{
		n--;
		d[n] = v;
	}

	return d;
}

/* dv_memcpy32() - copy an array.
 *
 * dv_memcpy32(unsigned *d, unsigned *s, unsigned n) copies an array of n 32-bit variables from s to d
*/
static inline unsigned *dv_memcpy32(unsigned *d, unsigned *s, unsigned n)
{
	while ( n > 0 )
	{
		n--;
		d[n] = s[n];
	}

	return d;
}

/* dv_strlen() - return length of a string
*/
static inline int dv_strlen(const char *str)
{
	const char *s = str;
	while ( *s != '\0' )
	{
		s++;
	}
	return (s-str);
}

/* Real functions
*/
extern unsigned *dv_memset32(unsigned *d, unsigned v, unsigned n);

#if 0
char *strcpy(char *d, const char *s);
char *strncpy(char *d, const char *s, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
#endif

#endif
