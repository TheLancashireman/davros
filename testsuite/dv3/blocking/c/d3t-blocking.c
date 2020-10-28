/* d3t-blocking.c - davros-3 test suite : test set for basic blocking functionality
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
#include "d3t.h"

extern void d3t_blocking_1(void);
extern void d3t_blocking_2(void);
extern void d3t_blocking_3(void);

/* d3t-testcase_init() - testcase initialization
 *
 *	- Called from d3t_init.
 *	- Create the davros objects that the test needs.
*/
void d3t_testcase_init(void)
{
	/* Nothing done here because creating executables is done during the tests.
	*/
}

/* d3t-controltask() - control the test sequence
 *
 *	- Started as a task after d3t_init terminates.
*/
void d3t_controltask(void)
{
	d3t_blocking_1();
	d3t_blocking_2();
	d3t_blocking_3();
	d3t_alldone("blocking");
}
