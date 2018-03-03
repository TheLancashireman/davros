/*	dv-allocatestack.c - allocate a stack for an executable
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-mempage.h>

DV_COVDEF(allocatestack);

/* dv_allocate_stack() - allocate a stack
 *
 * Find and allocate a stack for an executable.
 * Searches the executable table for another non-blockable executable with the same priority and shares
 * the stack.
 * There will be some exceptions to the sharing rule depending on the executable type.
*/
dv_mempage_t *dv_allocate_stack(dv_kernel_t *kvars, const dv_executable_t *exe)
{
	dv_executable_t *exe_tbl = dv_coreconfigs[kvars->core_index]->executables;
	dv_mempage_t *pge_tbl;
	int i;
	int pge_i;
	int n_pge;

	/* Ensure that no executable needs more than a page of stack.
	 * ToDo: for executables that need more than a page we'll have to do some spanning....
	*/
	if ( (exe->n_stack * sizeof(dv_stackword_t)) > (DV_MEM_PAGESIZE - DV_MEM_GAP) )
		dv_panic(dv_panic_unimplemented, "dv_allocate_stack", "executable stack bigger than a page");

	/* Can only share stack if the executable is non-blocking.
	*/
	if ( (exe->flags & (DV_EXEFLAG_EVENTS | DV_EXEFLAG_BLOCKING)) == 0 )
	{
		/* Search all the executables to find a stack to share.
		 * Cannot share with a blocking executable.
		 * Todo: this assumes all executables have the same stack size - i.e. one page. 
		*/
		for ( i=0; i<dv_coreconfigs[kvars->core_index]->n_executables; i++ )
		{
			if ( (exe_tbl[i].name != DV_NULL) &&
				 (exe_tbl[i].baseprio == exe->baseprio) &&
				 ((exe_tbl[i].flags && DV_EXEFLAG_BLOCKING) == 0) &&
				 (exe_tbl[i].stackpage != DV_NULL) )
			{
				DV_DBG(dv_kprintf("dv_allocate_stack(): %s shares stack 0x%08x with %s\n",
														exe->name, (unsigned)exe_tbl[i].stackpage, exe_tbl[i].name));
				exe_tbl[i].stackpage->n_use++;
				return exe_tbl[i].stackpage;
			}
		}
	}

	return dv_allocate_page(kvars);
}
