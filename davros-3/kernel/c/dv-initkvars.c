/*	dv-initkvars.c - initialise kernel variables for davros
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
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-stdio.h>

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
	dv_kprintf("     -- %d ring buffers\n",		ccfg->n_ringbuffers);
	dv_kprintf("     -- %d buffer words\n",		ccfg->n_ringbufferwords);
	dv_kprintf("     -- %d pages\n",			ccfg->n_pages);
	dv_kprintf("     -- %d pages\n",			ccfg->n_semaphores);

	kvars->current_thread = DV_NULL;
	kvars->kernel_sp = ccfg->kernelstacktop;
	kvars->core_index = ccfg->core_index;

	dv_dllinit(&kvars->thread_queue, dv_dll_priority);
	dv_dllinit(&kvars->sleep_queue, dv_dll_time);
	kvars->panic_reason[0] = kvars->panic_reason[1] = dv_panic_none;

	dv_init_allocator(&kvars->exe_allocator, ccfg->n_executables);
	dv_init_allocator(&kvars->thr_allocator, ccfg->n_threads);
	dv_init_allocator(&kvars->reg_allocator, ccfg->n_registers);
	dv_init_allocator(&kvars->evs_allocator, ccfg->n_events);
	dv_init_allocator(&kvars->dllelem_allocator, ccfg->n_dll_elements);
	dv_init_allocator(&kvars->rb_allocator, ccfg->n_ringbuffers);
	dv_init_allocator(&kvars->rbbuf_allocator, ccfg->n_ringbufferwords);
	dv_init_allocator(&kvars->page_allocator, ccfg->n_pages);
	dv_init_allocator(&kvars->sem_allocator, ccfg->n_semaphores);

	dv_set_kvars(kvars);
}
