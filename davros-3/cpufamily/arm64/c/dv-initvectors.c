/*	dv-initvectors.c - initialise vectors for ARM64 cpu
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <cpufamily/arm64/h/dv-arm64-vectors.h>
#include <cpufamily/arm64/h/dv-arm64-core.h>

DV_COVDEF(init_vectors);

/* dv_init_vectors() - initialise the ARM64 vector table.
 *
 * ARMv8 has a vector base address register. We just load it with the address of the vector table.
*/
void dv_init_vectors(void)
{
	dv_set_vbar_el1((dv_u64_t)dv_vectortable);
}
