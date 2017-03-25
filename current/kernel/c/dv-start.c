/*	dv-start.c - start davros
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
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-executable.h>

DV_COVDEF(start);

/* dv_start() - start Davros on one core
*/
void dv_start(dv_index_t ci)
{
	const dv_coreconfig_t *ccfg = dv_coreconfigs[ci];
	dv_kernel_t *kvars = ccfg->kernelvars;
	dv_index_t e;
	dv_executable_t *exe_tbl;

	DV_DBG(dv_kprintf("dv_start(): Initialising kvars 0x%08x for core %d\n", (unsigned)kvars, ci));
	dv_init_kvars(kvars, ccfg);

	exe_tbl = dv_coreconfigs[kvars->core_index]->executables;

	DV_DBG(dv_kprintf("dv_start(): Creating executable for idle thread core %d\n", ci));
	e = dv_create_executable(kvars, ccfg->idle_cfg);
	if ( e >= 0 )
	{
		exe_tbl[e].enabled = 1;
		DV_DBG(dv_kprintf("dv_start(): Spawning executable %d for idle thread core %d\n", e, ci));
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for idle thread");

	DV_DBG(dv_kprintf("dv_start(): Creating executable for init thread core %d\n", ci));
	e = dv_create_executable(kvars, ccfg->init_cfg);
	if ( e >= 0 )
	{
		exe_tbl[e].enabled = 1;
		DV_DBG(dv_kprintf("dv_start(): Spawning executable %d for init thread core %d\n", e, ci));
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for init thread");

	DV_DBG(dv_kprintf("dv_start(): Calling dv_init_vectors() on core %d\n", ci));
	dv_init_vectors();

	DV_DBG(dv_kprintf("dv_start(): Calling dv_dispatch() to start core %d\n", ci));
	dv_dispatch(kvars);
}

/* man-page-generation - to be defined
*/
