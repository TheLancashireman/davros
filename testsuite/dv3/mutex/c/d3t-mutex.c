/* d3t-mutex.c - davros-3 test suite : mutex
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
#include "d3t-mutex.h"

dv_index_t Foo, Bar, Quxx, Mutex;

const dv_execonfig_t foo_cfg =
{	.name		= "Foo",
	.function	= main_Foo,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 1,
	.flags		= 0
};

const dv_execonfig_t bar_cfg =
{	.name		= "Bar",
	.function	= main_Bar,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 2,
	.flags		= DV_EXEFLAG_BLOCKING
};

const dv_execonfig_t quxx_cfg =
{	.name		= "Bar",
	.function	= main_Quxx,
	.core		= 0,
	.n_stack	= 200,
	.priority	= 3,
	.flags		= DV_EXEFLAG_BLOCKING
};

/* d3t-testcase_init() - testcase initialization
 *
 *	- Called from d3t_init
 *	- Create the davros objects that the test needs
*/
void d3t_testcase_init(void)
{
	dv_dual_t rv;

	/* Create the three executables.
	*/
	rv = dv_create_exe(&foo_cfg);

	if ( rv.rv0 == dv_eid_None )
		Foo = rv.rv1;
	else
		dv_kprintf("Failed to create executable Foo: error = %d\n", rv.rv0);

	rv = dv_create_exe(&bar_cfg);

	if ( rv.rv0 == dv_eid_None )
		Bar = rv.rv1;
	else
		dv_kprintf("Failed to create executable Bar: error = %d\n", rv.rv0);

	rv = dv_create_exe(&quxx_cfg);

	if ( rv.rv0 == dv_eid_None )
		Quxx = rv.rv1;
	else
		dv_kprintf("Failed to create executable Quxx: error = %d\n", rv.rv0);
}

/* d3t-controltask() - control the test sequence
 *
 *	- Started as a task after d3t_init terminates
*/
void d3t_controltask(void)
{
	d3t_mutex_1();
	d3t_mutex_2();
	d3t_mutex_3();
	d3t_alldone("mutex");
}

/* main_Foo() - acquires the mutex then spawns the Bar executable
*/
void main_Foo(void)
{
	dv_errorid_t e;

	d3t_testpoint('F');

	e = dv_wait(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('G');
	else
		d3t_testpoint('@');

	e = dv_spawn(Bar);

	if ( e == dv_eid_None )
		d3t_testpoint('H');
	else
		d3t_testpoint('@');

	e = dv_signal(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('J');
	else
		d3t_testpoint('@');
}

/* main_Bar() - acquires the mutex
*/
void main_Bar(void)
{
	dv_errorid_t e;

	d3t_testpoint('B');

	e = dv_wait(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('C');
	else
		d3t_testpoint('#');

	e = dv_signal(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('D');
	else
		d3t_testpoint('#');
}

/* main_Quxx() - acquires the mutex
*/
void main_Quxx(void)
{
	dv_errorid_t e;

	d3t_testpoint('Q');

	e = dv_wait(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('R');
	else
		d3t_testpoint('$');

	e = dv_signal(Mutex);

	if ( e == dv_eid_None )
		d3t_testpoint('S');
	else
		d3t_testpoint('$');
}
