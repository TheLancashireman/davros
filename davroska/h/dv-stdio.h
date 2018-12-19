/*	dv_stdio.h - davroska stdio
 *
 *	Copyright 2018 David Haworth
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

#if !DV_ASM

#include <dv-xstdio.h>

typedef struct dv_uartdriver_s dv_uartdriver_t;

struct dv_uartdriver_s
{
	dv_xprintf_putc_t putc;		/* Put a character into the output stream. Wait if no room. */
	dv_xprintf_getc_t getc;		/* Get a character from the input stream. Wait if none. */
	dv_xprintf_getc_t istx;		/* Return TRUE if there's room in the output stream. */
	dv_xprintf_getc_t isrx;		/* Return TRUE if there's a character in input stream. */
};

extern dv_uartdriver_t dv_consoledriver;		/* Must be set up by the board init. */

/*	Functions for dv_printf.
*/
int dv_printf(const char *fmt, ...);
int dv_putc(int c);

#endif

#endif
