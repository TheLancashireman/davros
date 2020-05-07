/* dv_target_linux.h - header file for linux-hosted davroska
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/
#ifndef dv_target_linux_h
#define dv_target_linux_h	1

#include <stdio.h>
#include <setjmp.h>

typedef unsigned dv_intstatus_t;

typedef jmp_buf dv_jmpbuf_t;

#define dv_setjmp	setjmp
#define dv_longjmp	longjmp
#define dv_printf	printf

/* Cannot disable/restore interrupts on Linux
*/
#define DV_INTENABLED	0

static inline dv_intstatus_t dv_disable()
{
	return 0;
}

static inline dv_intstatus_t dv_restore(dv_intstatus_t unused_x)
{
	return 0;
}

#endif
