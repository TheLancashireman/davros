/*	dv-arm-vectors.h - ARM vector table for davros
 *
 *	Copyright 2017 David Haworth
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
#ifndef dv_arm_vectors_h
#define dv_arm_vectors_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#if !DV_ASM

/* dv_arm_vectors_t - a data structure that describes the vector table.
 *
 * See dv_vectors.s for the contents.
*/

typedef struct dv_arm_vectors_s dv_arm_vectors_t;

struct dv_arm_vectors_s
{
	dv_u32_t instruction[8];
	dv_u32_t address[8];
};

extern dv_arm_vectors_t dv_vectortable;

#endif

#define DV_VECTOR_RESET				0
#define DV_VECTOR_UNDEF				1
#define DV_VECTOR_SBREAK			2
#define DV_VECTOR_PREFETCH_ABORT	3
#define DV_VECTOR_DATA_ABPORT		4
#define DV_VECTOR_RESERVED			5
#define DV_VECTOR_IRQ				6
#define DV_VECTOR_FIQ				7

#endif
