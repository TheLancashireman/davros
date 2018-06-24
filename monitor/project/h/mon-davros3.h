/*	mon-davros3.h - monitor definitions for a davros-3 environment
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
 *	This file contains definitions for monitor.
 *
*/
#ifndef mon_davros3_h
#define mon_davros3_h

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-stdio.h>

#include <user/h/ctype.h>
#include <user/h/string.h>

typedef dv_u8_t uint8_t;
typedef dv_u16_t uint16_t;
typedef dv_u32_t uint32_t;
typedef dv_u64_t uint64_t;
typedef dv_address_t memaddr_t;
typedef dv_machineword_t maxword_t;

#define NULL	DV_NULL

static inline char readchar(void)
{
	return dv_consoledriver.getc();
}

static inline void writechar(char c)
{
	if ( c == '\n' )
		dv_consoledriver.putc('\r');
	dv_consoledriver.putc(c);
}

#define mprintf	dv_kprintf

#define xprintf(fmt, ap)	dv_xprintf(dv_consoledriver.putc, fmt, ap)

#endif
