/*	dv-arm64-vectors.h - ARM64 vector table for davros
 *
 *	Copyright 2018 David Haworth
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
#ifndef dv_arm64_vectors_h
#define dv_arm64_vectors_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>

#if !DV_ASM

/* dv_arm64_vectors_t - a data type that describes the vector table.
 *
 * On arm64 cpus there's always a vector base address register, so there's never any need to copy vector tables.
 * All we need is to put the address of the vector table in the appropriate VBAR.
 *
 * We only ever take the address of the vector table, so a simple address is sufficient.
 *
 * See dv_vectors.s for the contents.
*/

typedef dv_u64_t dv_arm_vectors_t;

extern dv_arm_vectors_t dv_vectortable;

#endif

#endif
