/*	dv-xstdio.h - stdio (subset of)
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
#ifndef dv_xstdio_h
#define dv_xstdio_h

#include <stdarg.h>

#ifndef NULL
#define NULL	((void *)0)
#endif

#define EOF (-1)

#define stdin	dv_getstdin()
#define stdout	dv_getstdout()
#define stderr	dv_getstderr()

int fprintf(void *stream, const char *fmt, ...);
int fgetc(void *stream);
int fputc(int c, void *stream);

#define printf(f...)	fprintf(stdout, ## f)
#define getc(s)			fgetc(s)

typedef int (*dv_xprintf_putc_t)(int);
typedef int (*dv_xprintf_getc_t)(void);

int dv_xprintf(dv_xprintf_putc_t xputc, const char *fmt, va_list ap);

#endif
