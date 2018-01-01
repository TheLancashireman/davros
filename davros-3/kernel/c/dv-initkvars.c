/*	dv-initkvars.c - initialise kernel variables for davros
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
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(initkvars);

/* dv_init_kvars() - initialise the kernel variables
*/
void dv_init_kvars(dv_kernel_t *kvars, const dv_coreconfig_t * ccfg)
{
	dv_kprintf("dv_init_kvars(): kvars = 0x%08x, ccfg = 0x%08x\n", kvars, ccfg);
	dv_kprintf("  -- kernel_sp = 0x%08x\n", ccfg->kernelstacktop);
	dv_kprintf("  -- Configuration\n");
	dv_kprintf("     -- %d executables\n",		ccfg->n_executables);
	dv_kprintf("     -- %d threads\n",			ccfg->n_threads);
	dv_kprintf("     -- %d register stores\n",	ccfg->n_registers);
	dv_kprintf("     -- %d event stores\n",		ccfg->n_events);
	dv_kprintf("     -- %d list elements\n",	ccfg->n_dll_elements);
	dv_kprintf("     -- %d pages\n",			ccfg->n_pages);

	kvars->current_thread = DV_NULL;
	kvars->kernel_sp = ccfg->kernelstacktop;
	kvars->core_index = ccfg->core_index;

	dv_dllinit(&kvars->thread_queue, dv_dll_priority);
	dv_dllinit(&kvars->sleep_queue, dv_dll_time);
	kvars->panic_reason[0] = kvars->panic_reason[1] = dv_panic_none;

	kvars->exe_allocator.n_free = ccfg->n_executables;
	kvars->exe_allocator.next = 0;
	kvars->thr_allocator.n_free = ccfg->n_threads;
	kvars->thr_allocator.next = 0;
	kvars->reg_allocator.n_free = ccfg->n_registers;
	kvars->reg_allocator.next = 0;
	kvars->evs_allocator.n_free = ccfg->n_events;
	kvars->evs_allocator.next = 0;
	kvars->dllelem_allocator.n_free = ccfg->n_dll_elements;
	kvars->dllelem_allocator.next = 0;
	kvars->page_allocator.n_free = ccfg->n_pages;
	kvars->page_allocator.next = 0;

	dv_set_kvars(kvars);
}

/* man-page-generation - to be defined
*/
