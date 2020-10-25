/* d3t-basics-2.c - davros-3 test suite : basics test 2
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
#include <kernel/h/dv-executable.h>
#include "d3t.h"

const dv_execonfig_t foo_2 =
{	.name		= "Foo",
	.function	= main_Foo,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 1,
	.flags		= DV_EXEFLAG_AUTODESTROY
};

const dv_execonfig_t bar_2 =
{	.name		= "Bar",
	.function	= main_Bar,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 2,
	.flags		= DV_EXEFLAG_AUTODESTROY
};

/* d3t_basics_2() - spawning a higher-priority executable
 *
 * Expect: abcdefghi
*/
void d3t_basics_2(void)
{
	dv_dual_t rv;
	dv_index_t id1;
	dv_errorid_t e;

	d3t_starttest("basics-2 - synchronous preemption()");
	d3t_expect("abcFGBHd");

	d3t_testpoint('a');

	rv = dv_create_exe(&foo_2);

	if ( rv.rv0 == dv_eid_None )
	{
		d3t_testpoint('b');

		id1 = rv.rv1;

		rv = dv_create_exe(&bar_2);

		if ( rv.rv0 == dv_eid_None )
		{
			d3t_testpoint('c');
			e = dv_spawn(id1);

			if ( e == dv_eid_None )
			{
				d3t_testpoint('d');
			}
			else
				d3t_testpoint('?');
		}
		else
			d3t_testpoint('?');
	}
	else
		d3t_testpoint('?');

	d3t_finishtest("basics-2");
}
