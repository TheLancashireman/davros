/*  dv-cortexm-vectors.c - vector table for Cortex-M devices
 *
 *  Copyright David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "dv-devices.h"

#ifndef DV_ARMv6_M
#define DV_ARMv6_M	0
#endif
#ifndef DV_ARMv7_M
#define DV_ARMv7_M	0
#endif

#ifndef DV_NVECTOR
#error "DV_NVECTOR not defined"
#elif DV_NVECTOR == 32 || DV_NVECTOR == 68
/* All the above values are supported. */
#else
#error "Unsupported value of DV_NVECTOR"
#endif

extern void dv_stacktop(void);	/* This is a blatant lie! dv_stacktop is a symbol set in the linker script */
extern void dv_reset(void);
extern void dv_nmi(void);
extern void dv_hardfault(void);

#if DV_ARMv7_M
extern void dv_memfault(void);
extern void dv_busfault(void);
extern void dv_usagefault(void);
#elif DV_ARMv6_M
#define dv_memfault		dv_unknowntrap
#define dv_busfault		dv_unknowntrap
#define dv_usagefault	dv_unknowntrap
#else
#error "Cortex-M architecture not defined"
#endif

extern void dv_svctrap(void);
extern void dv_pendsvtrap(void);
extern void dv_systickirq(void);
extern void dv_irq(void);
extern void dv_unknowntrap(void);

typedef void (*dv_vector_t)(void);

/* The vector table is an array of addresses.
 * This table has 84 vectors:
 *	- 16 are the armvx-m exception/reset vectors (including reset SP)
 *	- up to 68 are for the NVIC interrupts. They are all mappped to dv_irq
*/
const dv_vector_t dv_hwvectors[16+DV_NVECTOR] =
{	&dv_stacktop,
	&dv_reset,
	&dv_nmi,
	&dv_hardfault,
	&dv_memfault,
	&dv_busfault,
	&dv_usagefault,
	&dv_unknowntrap,
	&dv_unknowntrap,
	&dv_unknowntrap,
	&dv_unknowntrap,
	&dv_svctrap,
	&dv_unknowntrap,
	&dv_unknowntrap,
	&dv_pendsvtrap,
	&dv_systickirq,
	&dv_irq,	/* 00 */
	&dv_irq,	/* 01 */
	&dv_irq,	/* 02 */
	&dv_irq,	/* 03 */
	&dv_irq,	/* 04 */
	&dv_irq,	/* 05 */
	&dv_irq,	/* 06 */
	&dv_irq,	/* 07 */
	&dv_irq,	/* 08 */
	&dv_irq,	/* 09 */
	&dv_irq,	/* 10 */
	&dv_irq,	/* 11 */
	&dv_irq,	/* 12 */
	&dv_irq,	/* 13 */
	&dv_irq,	/* 14 */
	&dv_irq,	/* 15 */
	&dv_irq,	/* 16 */
	&dv_irq,	/* 17 */
	&dv_irq,	/* 18 */
	&dv_irq,	/* 19 */
	&dv_irq,	/* 20 */
	&dv_irq,	/* 21 */
	&dv_irq,	/* 22 */
	&dv_irq,	/* 23 */
	&dv_irq,	/* 24 */
	&dv_irq,	/* 25 */
	&dv_irq,	/* 26 */
	&dv_irq,	/* 27 */
	&dv_irq,	/* 28 */
	&dv_irq,	/* 29 */
	&dv_irq,	/* 30 */
	&dv_irq,	/* 31 */
#if DV_NVECTOR > 32
	&dv_irq,	/* 32 */
	&dv_irq,	/* 33 */
	&dv_irq,	/* 34 */
	&dv_irq,	/* 35 */
	&dv_irq,	/* 36 */
	&dv_irq,	/* 37 */
	&dv_irq,	/* 38 */
	&dv_irq,	/* 39 */
	&dv_irq,	/* 40 */
	&dv_irq,	/* 41 */
	&dv_irq,	/* 42 */
	&dv_irq,	/* 43 */
	&dv_irq,	/* 44 */
	&dv_irq,	/* 45 */
	&dv_irq,	/* 46 */
	&dv_irq,	/* 47 */
	&dv_irq,	/* 48 */
	&dv_irq,	/* 49 */
	&dv_irq,	/* 50 */
	&dv_irq,	/* 51 */
	&dv_irq,	/* 52 */
	&dv_irq,	/* 53 */
	&dv_irq,	/* 54 */
	&dv_irq,	/* 55 */
	&dv_irq,	/* 56 */
	&dv_irq,	/* 57 */
	&dv_irq,	/* 58 */
	&dv_irq,	/* 59 */
	&dv_irq,	/* 60 */
	&dv_irq,	/* 61 */
	&dv_irq,	/* 62 */
	&dv_irq,	/* 63 */
	&dv_irq,	/* 64 */
	&dv_irq,	/* 65 */
	&dv_irq,	/* 66 */
	&dv_irq,	/* 67 */
#endif
};
