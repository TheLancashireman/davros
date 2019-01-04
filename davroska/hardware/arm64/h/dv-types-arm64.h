/*	dv-types-arm64.h - header file for davroska; ARM64 data types
 *
 *	Copyright 2019 David Haworth
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
#ifndef dv_types_arm64_h
#define dv_types_arm64_h	1

/* ARM64 is a standard 64-bit processor
*/
#include <dv-types-64.h>
#include <dv-types.h>
#include <arm64/h/dv-arm64-registers.h>

/* Use the compiler's setjmp/longjmp
*/
#include <setjmp.h>

typedef jmp_buf dv_jmpbuf_t;

#define dv_setjmp	setjmp
#define dv_longjmp	longjmp

/* Interrupt status, locking and unlocking
*/
typedef dv_u64_t dv_intstatus_t;

static inline dv_intstatus_t dv_disable()
{
	dv_intstatus_t old = dv_arm64_mrs(DAIF);
	dv_arm64_msr(DAIF, old|DV_SPSR_I);
    return old;
}

static inline dv_intstatus_t dv_restore(dv_intstatus_t x)
{
	dv_intstatus_t old = dv_arm64_mrs(DAIF);
	dv_arm64_msr(DAIF, ((old&~DV_SPSR_I)|(x&DV_SPSR_I)));
    return old;
}

#endif
