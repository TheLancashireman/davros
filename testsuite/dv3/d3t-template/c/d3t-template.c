/* d3t-template.c - davros-3 test suite : template test set
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

/* d3t-testcase_init() - testcase initialization
 *
 *	- Called from d3t_init
 *	- Create the davros objects that the test needs
*/
void d3t_testcase_init(void)
{
}

/* d3t-controltask() - control the test sequence
 *
 *	- Started as a task after d3t_init terminates
*/
void d3t_controltask(void)
{
	d3t_starttest("Template-1");
	d3t_testpoint('a');
	d3t_testpoint('b');
	d3t_testpoint('c');
	d3t_finishtest("Template-1");
	d3t_alldone("Template");
}
