/*	dv-allocateregisters.c - allocate/deallocate a register structure for an executable
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
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include DV_H_REGISTERS

DV_COVDEF(allocateregisters);

/* dv_is_free_registers() - return true if register structure i is free
*/
static dv_boolean_t dv_is_free_registers(dv_index_t i, const void *tbl)
{
	const dv_registers_t *reg_tbl = tbl;
	if ( reg_tbl[i].n_exe == 0 )
		return 1;
	return 0;
}

/* dv_allocate_registers() - allocate a register structure
 *
 * Find and allocate a register structure for an executable.
 * Searches the executable table for another non-blockable executable with the same priority and shares
 * the registers.
*/
dv_registers_t *dv_allocate_registers(dv_kernel_t *kvars, const dv_executable_t *exe)
{
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_registers_t *reg_tbl;
	int i;
	int reg_i;
	int n_reg;

	/* Can only share registers if the executable is non-blocking.
	*/
	if ( (exe->flags & DV_EXEFLAG_BLOCKING) == 0 )
	{
		/* Search all the executables to find a registers to share.
		 * Cannot share with a blocking executable.
		*/
		for ( i=0; i<dv_coreconfigs[kvars->core_index]->n_executables; i++ )
		{
			if ( (exe_tbl[i].name != DV_NULL) &&
				 (exe_tbl[i].baseprio == exe->baseprio) &&
				 ((exe_tbl[i].flags && DV_EXEFLAG_BLOCKING) == 0) &&
				 (exe_tbl[i].registers != DV_NULL) )
			{
				DV_DBG(dv_kprintf("dv_allocate_registers(): %s shares registers 0x%08x with %s\n",
														exe->name, (unsigned)exe_tbl[i].registers, exe_tbl[i].name));
				exe_tbl[i].registers->n_exe++;
				return exe_tbl[i].registers;
			}
		}
	}

	reg_tbl = dv_coreconfigs[kvars->core_index]->registers;
	n_reg = dv_coreconfigs[kvars->core_index]->n_registers;
	reg_i = dv_allocate_obj(&kvars->reg_allocator, n_reg, dv_is_free_registers, reg_tbl);

	if ( reg_i < 0 )
		return DV_NULL;

	reg_tbl[reg_i].n_exe = 1;

	return &reg_tbl[reg_i];
}

/* dv_deallocate_registers() - deallocate a register structure
 *
 * Decrement the occupation counter; if zero, deallocate the object.
*/
void dv_deallocate_registers(dv_kernel_t *kvars, dv_registers_t *reg)
{
	dv_assert((reg->n_exe > 0), dv_panic_initialisationerror,
			"dv_deallocate_registers", "register structure is not occupied");

	/* Decrement the occupation counter
	*/
	reg->n_exe--;

	/* Free the object when there are no more occupiers
	*/
	if ( reg->n_exe <= 0 )
	{
		dv_deallocate_obj(&kvars->reg_allocator);
	}
}
