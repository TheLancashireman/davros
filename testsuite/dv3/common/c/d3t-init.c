/* dt3-init.c - davros-3 test suite initialization task
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
*/
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-error.h>
#include "dt3.h"

/* Control task configuration
*/
const dv_execonfig_t dt3_controltask_cfg =
{   "dt3_controltask",	/* Task name */
	dt3_controltask,	/* Task function */
	0,					/* Core */
	200,				/* Stacksize (words) */
	0,					/* Priority */
	0					/* Flags */
};

/* dt3-init() - initialization task
*/
void dt3_init(void)
{
	dv_errorid_t e;

	if ( dt3_coreindex() == 0 )
	{
		dv_dual_t rv;
		rv = dv_create_exe(&dt3_controltask_cfg);

		if ( rv.rv0 == dv_eid_None )
		{
			e = dv_spawn((dv_index_t)rv.rv1);

			if ( e != dv_eid_None )
				dt3_abort("dt3_init() : couldn't spawn dt3_controltsask", e);
		}
		else
			dt3_abort("dt3_init() : couldn't create dt3_controltsask", rv.rv0);
	}

	dt3_testcase_init();
}
