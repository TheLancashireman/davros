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
#include DV_START

DV_COVDEF(start);

/* dv_start() - start Davros on one core
*/
void dv_start(dv_index_t ci)
{
	const dv_coreconfig_t *ccfg = dv_coreconfigs[ci];
	dv_kernel_t *kvars = ccfg->kernelvars;
	dv_index_t e;
	dv_executable_t *exe_tbl;

	/* Initialise the kernel variables.
	*/
	dv_init_kvars(kvars, ccfg);

	/* Initialise the processor's hardware.
	*/
	dv_init_hardware(kvars);

	/* Initialise the interrupt vectoring
	*/
	dv_init_vectors();

	/* Initialise the peripherals that davros uses.
	*/
	dv_init_peripherals(kvars);

	/* Create executables for main() and the idle loop, and spawn them.
	*/
	exe_tbl = dv_coreconfigs[kvars->core_index]->executables;

	e = dv_create_executable(kvars, ccfg->idle_cfg);
	if ( e >= 0 )
	{
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for idle thread");

	e = dv_create_executable(kvars, ccfg->init_cfg);
	if ( e >= 0 )
	{
		dv_spawn_executable(kvars, &exe_tbl[e]);
	}
	else
		dv_panic(dv_panic_objectsearchfailed, "dv_start", "Failed to create executable for init thread");

#if DV_PRJ_STARTUP
	/* Optional: call the project's startup function.
	*/
	prj_startup(kvars);
#endif

	/* Dispatch the highest-priority thread.
	*/
	dv_dispatch(kvars);
}

/* man-page-generation - to be defined
*/
