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

extern void d3t_basics_1(void);
extern void d3t_basics_2(void);
extern void d3t_basics_3(void);
extern void d3t_basics_4(void);

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
	d3t_basics_2();
	d3t_basics_3();
#if 0
	d3t_basics_4();
#endif
	d3t_alldone("basics");
}

/* main_Foo() - spawns the executable with the next higher ID
*/
void main_Foo(void)
{
	d3t_testpoint('F');

	dv_index_t myId = dv_get_exeid();

	d3t_testpoint('G');

	dv_errorid_t e = dv_spawn(myId+1);

	if ( e == dv_eid_None )
		d3t_testpoint('H');
	else
		d3t_testpoint('@');
}

/* main_Bar() - records a testpoint
*/
void main_Bar(void)
{
	d3t_testpoint('B');
}
