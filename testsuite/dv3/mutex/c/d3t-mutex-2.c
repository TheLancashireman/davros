/* d3t-mutex-2.c - davros-3 test suite : mutex test 2
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

/* d3t_mutex_2() - spawning a higher-priority executable while holding a mutex
*/
void d3t_mutex_2(void)
{
	dv_dual_t rv;
	dv_errorid_t e;

	d3t_starttest("mutex-2 - preemption while holding a mutex");
	d3t_expect("a");

	d3t_testpoint('a');

	rv = dv_create_semaphore(dv_semaphore_immediateceiling, 1);

	if ( rv.rv0 == dv_eid_None )
	{
		d3t_testpoint('b');

		Mutex = rv.rv1;

		e = dv_uses_semaphore(Mutex, Foo);

		if ( e == dv_eid_None )
			d3t_testpoint('c');
		else
			d3t_testpoint('?');

		e = dv_uses_semaphore(Mutex, Bar);

		if ( e == dv_eid_None )
			d3t_testpoint('d');
		else
			d3t_testpoint('?');

		e = dv_uses_semaphore(Mutex, Quxx);

		if ( e == dv_eid_None )
			d3t_testpoint('e');
		else
			d3t_testpoint('?');

		e = dv_spawn(Foo);

		if ( e == dv_eid_None )
			d3t_testpoint('f');
		else
			d3t_testpoint('?');

		e = dv_destroy_semaphore(Mutex);

		if ( e == dv_eid_None )
			d3t_testpoint('g');
		else
			d3t_testpoint('?');
	}
	else
		d3t_testpoint('?');

	d3t_finishtest("mutex-2");
}
