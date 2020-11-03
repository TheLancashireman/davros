/* d3t-init.c - davros-3 test suite initialization task
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
#include "d3t.h"

char d3t_testsequence[D3T_MAX_SEQ+1];
int d3t_seqcount;
char *d3t_expected;
int d3t_npass;
int d3t_nfail;

/* Control task configuration
*/
const dv_execonfig_t d3t_controltask_cfg =
{   "d3t_controltask",	/* Task name */
	d3t_controltask,	/* Task function */
	0,					/* Core */
	200,				/* Stacksize (words) */
	0,					/* Priority */
	0					/* Flags */
};

/* d3t-init() - initialization task
*/
void d3t_init(void)
{
	dv_errorid_t e;

	if ( d3t_coreindex() == 0 )
	{
		dv_dual_t rv;
		rv = dv_create_exe(&d3t_controltask_cfg);

		if ( rv.rv0 == dv_eid_None )
		{
			e = dv_spawn((dv_index_t)rv.rv1);

			if ( e != dv_eid_None )
				d3t_abort("d3t_init() : couldn't spawn d3t_controltask", e);
		}
		else
			d3t_abort("d3t_init() : couldn't create d3t_controltask", rv.rv0);
	}

	d3t_testcase_init();
}
