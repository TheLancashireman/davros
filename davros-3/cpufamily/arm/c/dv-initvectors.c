/*	dv-initvectors.c - initialise vectors for ARM cpu
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <cpufamily/arm/h/dv-arm-vectors.h>

DV_COVDEF(init_vectors);

/* dv_init_vectors() - initialise the ARM vector table.
 *
 * If there's RAM at address 0 we copy the vectors there.
 * If the memory at 0 is read-only we have to ensure that there's a vector table that
 * redirects somewhere else, and copy the vectors to the redirection table.
 * Either way, the constant dv_vector_location contains the address.
*/
void dv_init_vectors(void)
{
	((dv_arm_vectors_t *)dv_vector_location)[0] = dv_vectortable;
}
