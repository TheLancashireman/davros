/* d3t-blocking-3.c - davros-3 test suite : blocking test 3
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
#include <kernel/h/dv-api.h>
#include "d3t.h"

void main_Foo3(void);

const dv_execonfig_t foo_3 =
{   .name       = "Foo",
    .function   = main_Foo3,
    .core       = 0,
    .n_stack    = 200,
    .priority   = 1,
    .flags      = DV_EXEFLAG_BLOCKING
};

/* d3t_blocking_3() - error returns from dv_sleep() and dv_sleep_until()
*/
void d3t_blocking_3(void)
{
	dv_dual_t rv;

	d3t_starttest("blocking-3 - dv_suspend() and dv_resume()");
	d3t_expect("aFbGc");

	rv = dv_create_exe(&foo_3);

	if ( rv.rv0 == dv_eid_None )
	{
		d3t_testpoint('a');

		if ( dv_spawn(rv.rv1) == dv_eid_None )
		{
			d3t_testpoint('b');

			if ( dv_resume(rv.rv1) == dv_eid_None )
				d3t_testpoint('c');
			else
				d3t_testpoint('?');
		}
		else
			d3t_testpoint('?');
	}
	else
		d3t_testpoint('?');

	d3t_finishtest("blocking-3");
}

void main_Foo3(void)
{
	d3t_testpoint('F');

	if ( dv_suspend() == dv_eid_None )
		d3t_testpoint('G');
	else
		d3t_testpoint('?');
}
