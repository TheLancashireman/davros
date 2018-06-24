/*	monitor.h - monitor definitions
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

#ifndef monitor_h
#define monitor_h

#define MON_DAVROS3		1
#define MON_LINUXTEST	99

#ifndef MON_ENVIRONMENT
#define MON_ENVIRONMENT	MON_DAVROS3
#endif

#if MON_ENVIRONMENT == MON_DAVROS3
#include <project/h/mon-davros3.h>
#else
#error "Unknown value of MON_ENVIRONMENT; check your Makefiles"
#endif

#define MAXLINE	1024

#define SREC_EOF		1
#define SREC_BADTYP		-1
#define SREC_BADLEN		-2
#define SREC_NONHEX		-3
#define SREC_BADCK		-4

#define	peek8(a)		(*(uint8_t *)(a))
#define	peek16(a)		(*(uint16_t *)(a))
#define	peek32(a)		(*(uint32_t *)(a))
#define	peek64(a)		(*(uint64_t *)(a))
#define poke8(a, v)		(*(uint8_t *)(a) = (v))
#define poke16(a, v)	(*(uint16_t *)(a) = (v))
#define poke32(a, v)	(*(uint32_t *)(a) = (v))
#define poke64(a, v)	(*(uint64_t *)(a) = (v))
#define go(a)			((*(vfuncv_t)(a))())

typedef void (*pokefunc_t)(memaddr_t a, uint8_t b);
typedef void (*vfuncv_t)(void);

void monitor(char *prompt);

int process_s_record(char *line, pokefunc_t _poke);

#if 0
int mprintf(char *fmt, ...);
#endif

char *mgets(char *buf, int max);

char *skipspaces(char *p);
int char2hex(char c);
maxword_t gethex(char **pp, int max);

#endif
