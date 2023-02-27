/*	dv-printf.c - davroska: formatted output to terminal (polled driver)
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
#include <dv-config.h>
#include <dv-xstdio.h>
#include <dv-stdio.h>
#include <davroska.h>

#include DV_TARGET

#include "stdarg.h"

dv_uartdriver_t dv_consoledriver;		/* Must be set up by the board init. */
short dv_printf_mutex = -1;

int dv_printf(const char *fmt, ...)
{
	int nprinted;
	va_list ap;
	dv_intstatus_t is = 0;

	if ( dv_printf_mutex < 0 )
		is = dv_disable();
	else
	if ( dv_takemutex(dv_printf_mutex) != dv_e_ok )
		return -1;

	va_start(ap, fmt);
	nprinted = dv_xprintf(dv_putc, fmt, ap);
	va_end(ap);

	if ( dv_printf_mutex < 0 )
		dv_restore(is);
	else
		(void)dv_dropmutex(dv_printf_mutex);

	return(nprinted);
}

int dv_putc(int c)
{
	if ( c == '\n' )
		dv_consoledriver.putc('\r');
	return dv_consoledriver.putc(c);
}
