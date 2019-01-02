/*	dv-printf.c - davroska: formatted output to terminal (polled driver)
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
 *	This file contains kprintf. kprintf prints formatted output
 *	on the console using the driver's polled mode.
 *
*/
#include <dv-config.h>
#include <dv-xstdio.h>
#include <dv-stdio.h>

#include DV_TARGET

#include "stdarg.h"

dv_uartdriver_t dv_consoledriver;		/* Must be set up by the board init. */

int dv_printf(const char *fmt, ...)
{
	int nprinted;
	va_list ap;

	dv_intstatus_t is = dv_disable();

	va_start(ap, fmt);
	nprinted = dv_xprintf(dv_putc, fmt, ap);
	va_end(ap);

	dv_restore(is);

	return(nprinted);
}

int dv_putc(int c)
{
	if ( c == '\n' )
		dv_consoledriver.putc('\r');
	return dv_consoledriver.putc(c);
}
