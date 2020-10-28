/*	dv-kprintf.c - formatted output to terminal (polled driver)
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
 *
 *	This file contains kprintf. kprintf prints formatted output
 *	on the console using the driver's polled mode.
 *
*/
#include <kernel//h/dv-kconfig.h>
#include <dv-xstdio.h>
#include <kernel/h/dv-stdio.h>

#include "stdarg.h"

dv_uartdriver_t dv_consoledriver;		/* Must be set up by the board init. */

int dv_kprintf(const char *fmt, ...)
{
	int nprinted;
	va_list ap;
#if 0
	int modesave;
#endif

	va_start(ap, fmt);

#if 0
	modesave = kmode(KPOLLED);
#endif
	nprinted = dv_xprintf(dv_kputc, fmt, ap);
#if 0
	kmode(modesave);
#endif
	va_end(ap);

	return(nprinted);
}

int dv_kputc(int c)
{
	if ( c == '\n' )
		dv_consoledriver.putc('\r');
	return dv_consoledriver.putc(c);
}
