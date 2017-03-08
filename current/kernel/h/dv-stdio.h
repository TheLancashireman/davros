/*	dv_stdio.h - kernel stdio
 *
 *	Copyright 2001 David Haworth
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
 *
 *	This file contains the "stdio" functions
 *
*/
#ifndef dv_stdio_h
#define dv_stdio_h

#include "stdarg.h"

typedef int (*dv_xprintf_putc)(int,void *);

int dv_xprintf(dv_xprintf_putc, void *, const char *, va_list);

/*	Functions for kprintf.
 *	kprintf is in the library
 *	kputc must be provided by the board drivers. It puts a single character
 *	to the console (in polled mode).
 *	kmode must be provided by the board drivers. It puts the console into
 *	the given mode and returns the old mode. The only specified value
 *	of mode is 0 (KPOLLED)
*/
int dv_kprintf(const char *, ...);
int dv_kputc(int, void *);
int dv_strlen(const char *);

#if 0
int dv_kmode(int m);
#define KPOLLED 0
#define KINTR	1
#endif

#endif
