/*	dv-types-arm.h - header file for davroska; ARM data types
 *
 *	Copyright David Haworth
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
#ifndef dv_types_arm_h
#define dv_types_arm_h	1

/* ARM is a standard 32-bit processor
*/
#include <dv-types-32.h>
#include <dv-types.h>
#include <arm/h/dv-arm-registers.h>

/* Use the compiler's setjmp/longjmp
*/
#include <setjmp.h>

typedef jmp_buf dv_jmpbuf_t;

#define dv_setjmp	setjmp
#define dv_longjmp	longjmp

/* Interrupt status, locking and unlocking
*/
typedef dv_u32_t dv_intstatus_t;

static inline dv_intstatus_t dv_disable()
{
	dv_u32_t old = dv_arm_mrs(cpsr);
	dv_arm_msr(cpsr_c, old|DV_ARM_IRQ_DIS);
    return old;
}

static inline dv_intstatus_t dv_restore(dv_intstatus_t x)
{
	dv_u32_t old = dv_arm_mrs(cpsr);
	dv_arm_msr(cpsr_c, ((old&~DV_ARM_IRQ_DIS)|(x&DV_ARM_IRQ_DIS)));
    return old;
}

#endif
