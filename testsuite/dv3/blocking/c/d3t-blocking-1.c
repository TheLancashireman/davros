/* d3t-blocking-1.c - davros-3 test suite : blocking test 1
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
#include <kernel//h/dv-api.h>
#include "d3t.h"

void main_Foo1(void);

const dv_execonfig_t foo_1 =
{   .name       = "Foo",
    .function   = main_Foo1,
    .core       = 0,
    .n_stack    = 200,
    .priority   = 1,
    .flags      = DV_EXEFLAG_AUTODESTROY | DV_EXEFLAG_BLOCKING
};

/* d3t_blocking_1() - error returns from dv_sleep(), dv_sleep_until(), dv_suspend() and dv_resume()
*/
void d3t_blocking_1(void)
{
	dv_dual_t rv;

	d3t_starttest("blocking-1 - errors from dv_sleep(), dv_sleep_until(), dv_suspend() and dv_resume()");
	d3t_expect("abcdefghFGHi");

	/* The control task is non-blocking, so test those errors here.
	*/
	if ( dv_sleep(1000000) == dv_eid_ExecutableIsNonBlocking )
		d3t_testpoint('a');
	else
		d3t_testpoint('?');

	if ( dv_sleep_until(1000000) == dv_eid_ExecutableIsNonBlocking )
		d3t_testpoint('b');
	else
		d3t_testpoint('?');

	if ( dv_suspend() == dv_eid_ExecutableIsNonBlocking )
		d3t_testpoint('c');
	else
		d3t_testpoint('?');

	if ( dv_resume(-1) == dv_eid_IndexOutOfRange )
		d3t_testpoint('d');
	else
		d3t_testpoint('?');

	if ( dv_resume(DV_C0_N_EXECUTABLES) == dv_eid_IndexOutOfRange )
		d3t_testpoint('e');
	else
		d3t_testpoint('?');

	if ( dv_resume(DV_C0_N_EXECUTABLES-1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('f');
	else
		d3t_testpoint('?');

	rv = dv_create_exe(&foo_1);

	if ( rv.rv0 == dv_eid_None )
	{
		d3t_testpoint('g');

		if ( dv_resume(rv.rv1) == dv_eid_ExecutableNotSuspended )
			d3t_testpoint('h');
		else
			d3t_testpoint('?');

		if ( dv_spawn(rv.rv1) == dv_eid_None )
		{
			d3t_testpoint('i');
		}
		else
			d3t_testpoint('?');
	}
	else
		d3t_testpoint('?');

	d3t_finishtest("blocking-1");
}

void main_Foo1(void)
{
	dv_u64_t t1;

	d3t_testpoint('F');

	if ( dv_sleep(DV_MIN_SLEEP-1) == dv_eid_TimeInThePast )
		d3t_testpoint('G');
	else
		d3t_testpoint('?');

	t1 = dv_readtime();

	if ( dv_sleep_until(t1-1) == dv_eid_TimeInThePast )
		d3t_testpoint('H');
	else
		d3t_testpoint('?');
}
