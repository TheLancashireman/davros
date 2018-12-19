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
#include <dv-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-interrupt.h>
#include DV_H_START

DV_COVDEF(start);

/* dv_start() - start Davros on one core
*/
void dv_start(dv_index_t ci)
{
	const dv_coreconfig_t *ccfg = dv_coreconfigs[ci];
	dv_kernel_t *kvars = ccfg->kernelvars;
	dv_index_t e;
	dv_executable_t *exe_tbl;

	DV_DBG(dv_kprintf("dv_start(%d)\n", ci));

	/* Initialise the kernel variables.
	*/
	dv_init_kvars(kvars, ccfg);

	/* Initialise the processor's hardware.
	*/
	dv_init_hardware(kvars);

	/* Initialise the exception and interrupt vectoring
	*/
	dv_init_vectors();
	dv_init_softvector();

	/* Initialise the peripherals that davros uses.
	*/
	dv_init_peripherals(kvars);

	/* Create executables for main() and the idle loop, and spawn them.
	*/
	exe_tbl = dv_coreconfigs[kvars->core_index]->executables;

	DV_DBG(dv_kprintf("dv_create_executable(idle)\n"));
	e = dv_create_executable(kvars, ccfg->idle_cfg);
	if ( e >= 0 )
	{
		DV_DBG(dv_kprintf("dv_spawn_executable(%d)\n", e));
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for idle");

	DV_DBG(dv_kprintf("dv_create_executable(init)\n"));
	e = dv_create_executable(kvars, ccfg->init_cfg);
	if ( e >= 0 )
	{
		DV_DBG(dv_kprintf("dv_spawn_executable(%d)\n", e));
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for init");

#if DV_PRJ_STARTUP
	/* Optional: call the project's startup function.
	*/
	prj_startup(kvars);
#endif

	/* Dispatch the highest-priority thread.
	*/
	dv_dispatch(kvars);
}
