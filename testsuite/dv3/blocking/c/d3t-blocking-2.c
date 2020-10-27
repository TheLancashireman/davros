/* d3t-blocking-2.c - davros-3 test suite : blocking test 2
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

void main_Foo2(void);

const dv_execonfig_t foo_2 =
{   .name       = "Foo",
    .function   = main_Foo2,
    .core       = 0,
    .n_stack    = 200,
    .priority   = 1,
    .flags      = DV_EXEFLAG_BLOCKING
};

volatile int foo2_state;

dv_u32_t d1, d2;

/* d3t_blocking_2() - error returns from dv_sleep() and dv_sleep_until()
*/
void d3t_blocking_2(void)
{
	dv_dual_t rv;

	d3t_starttest("blocking-2 - dv_sleep() and dv_sleep_until()");
	d3t_expect("aFbGHcIJd");

	rv = dv_create_exe(&foo_2);

	if ( rv.rv0 == dv_eid_None )
	{
		d3t_testpoint('a');

		foo2_state = 0;

		if ( dv_spawn(rv.rv1) == dv_eid_None )
		{
			d3t_testpoint('b');

			while ( foo2_state == 0 )
			{
			}

			d3t_testpoint('c');

			while ( foo2_state == 1 )
			{
			}

			d3t_testpoint('d');
		}
		else
			d3t_testpoint('?');
	}
	else
		d3t_testpoint('?');

	d3t_finishtest("blocking-2");
	dv_kprintf("d1 = %u, d2 = %u\n", d1, d2);
}

#define DELAY	1000000

void main_Foo2(void)
{
	dv_u64_t t1, t2, d;
	dv_errorid_t e;

	t1 = dv_readtime();

	d3t_testpoint('F');

	e = dv_sleep(DELAY);
	t2 = dv_readtime();

	if ( e == dv_eid_None )
		d3t_testpoint('G');
	else
		d3t_testpoint('?');

	d = t2 - t1;
	if ( d < DELAY )
		d3t_testpoint('<');
	else if ( d > (DELAY+100) )
		d3t_testpoint('>');
	else
		d3t_testpoint('H');

	d1 = (d > 0xffffffff) ? 0xffffffff : d;

	foo2_state = 1;

	e = dv_sleep_until(t2+DELAY);
	t1 = dv_readtime();

	if ( e == dv_eid_None )
		d3t_testpoint('I');
	else
		d3t_testpoint('?');

	d = t1 - t2;
	if ( d < DELAY )
		d3t_testpoint('<');
	else if ( d > DELAY+20 )
		d3t_testpoint('>');
	else
		d3t_testpoint('J');

	d2 = (d > 0xffffffff) ? 0xffffffff : d;

	foo2_state = 2;
}
