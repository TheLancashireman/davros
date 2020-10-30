/* d3t-mutex-3.c - davros-3 test suite : mutex test 3
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
#include "d3t-mutex.h"

const dv_execonfig_t foo_3 =
{	.name		= "Foo",
	.function	= main_Foo,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 1,
	.flags		= DV_EXEFLAG_AUTODESTROY
};

const dv_execonfig_t bar_3 =
{	.name		= "Bar",
	.function	= main_Bar,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 1,
	.flags		= DV_EXEFLAG_AUTODESTROY
};

/* d3t_mutex_3() - ToDo
*/
void d3t_mutex_3(void)
{
	d3t_starttest("mutex-3 - TBD");
	d3t_expect("a");
	d3t_testpoint('a');
	d3t_finishtest("mutex-3");
}
