/*	dv-initvectors.c - initialise vectors for ARM64 cpu
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <cpufamily/arm64/h/dv-arm64-vectors.h>
#include <dv-arm64-core.h>
#include <kernel//h/dv-stdio.h>

DV_COVDEF(init_vectors);

/* dv_init_vectors() - initialise the ARM64 vector table.
 *
 * ARMv8 has a vector base address register. We just load it with the address of the vector table.
*/
void dv_init_vectors(void)
{
	dv_u64_t vba = (dv_u64_t)&dv_vectortable;
	dv_kprintf("Setting VBAR_EL1 to 0x%08x%08x\n", (dv_u32_t)(vba>>32), (dv_u32_t)(vba));
	dv_set_vbar_el1(vba);
}
