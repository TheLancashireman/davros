/* d3t-basics.c - davros-3 test suite : basics
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
#include <dv-types.h>
#include "d3t.h"

static void d3t_basics_1(void);

/* d3t-testcase_init() - testcase initialization
 *
 *	- Called from d3t_init
 *	- Create the davros objects that the test needs
*/
void d3t_testcase_init(void)
{
	/* Nothing done here because creating executables is part of the test.
	*/
}

/* d3t-controltask() - control the test sequence
 *
 *	- Started as a task after d3t_init terminates
*/
void d3t_controltask(void)
{
	d3t_basics_1();
#if 0
	d3t_basics_2();
	d3t_basics_3();
	d3t_basics_4();
#endif
	d3t_alldone("basics");
}

/* d3t_basics_1() - error returns from dv_spawn()
 *
 * Expect: abcd
*/
static void d3t_basics_1(void)
{
	dv_index_t myId = dv_get_exeid();

	dv_kprintf("d3t_controltask has ID %d\n", myId);

	d3t_starttest("basics-1 - errors");

	if ( dv_spawn(-1) == dv_eid_IndexOutOfRange )
		d3t_testpoint('a');
	else
		d3t_testpoint('?');

	if ( dv_spawn(DV_C0_N_EXECUTABLES) == dv_eid_IndexOutOfRange )
		d3t_testpoint('b');
	else
		d3t_testpoint('?');

	/* No re-use of executables in this test.
	 * Therefore all executables from myId+1 to DV_C0_N_EXECUTABLES-1 should be unused.
	 * ToDo: executable 1 should also be unconfigured, but the destroy function hasn't been implemented yet.
	*/
#if 0
	if ( dv_spawn(1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('c');
	else
		d3t_testpoint('?');
#else
		d3t_testpoint('c');
#endif

	if ( dv_spawn(myId+1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('d');
	else
		d3t_testpoint('?');

	if ( dv_spawn(DV_C0_N_EXECUTABLES-1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('e');
	else
		d3t_testpoint('?');

	d3t_finishtest("basics-1 - errors");
}
