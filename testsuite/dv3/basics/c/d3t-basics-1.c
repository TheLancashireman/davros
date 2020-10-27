/* d3t-basics-1.c - davros-3 test suite : basics test 1
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

/* d3t_basics_1() - error returns from dv_spawn()
*/
void d3t_basics_1(void)
{
	dv_index_t myId = dv_get_exeid();

	dv_kprintf("d3t_controltask has ID %d\n", myId);

	d3t_starttest("basics-1 - errors from dv_spawn()");
	d3t_expect("abcdefghi");

	if ( dv_spawn(-1) == dv_eid_IndexOutOfRange )
		d3t_testpoint('a');
	else
		d3t_testpoint('?');

	if ( dv_spawn(DV_C0_N_EXECUTABLES) == dv_eid_IndexOutOfRange )
		d3t_testpoint('b');
	else
		d3t_testpoint('?');

	/* Executables are allocated in ID sequence. Old IDs are reused after the last in the array has been allocated.
	 * In this test:
	 *	ID 0 is the idle thread - still active.
	 *	ID 1 is the init thread - terminated.
	 *  ...
	 *  myId is the current executable
	 * Therefore executables from 1 to myId-1 and from myId+1 to DV_C0_N_EXECUTABLES-1 should be unused.
	*/
	if ( dv_spawn(1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('c');
	else
		d3t_testpoint('?');

	/* myId-1 should be the same as 1, but we test the boundary anyway.
	*/
	// ToDo: executable myId-1 should be free, but the destroy functionality hasn't been implemented yet.
	if ( dv_spawn(myId-1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('d');
	else
		d3t_testpoint('?');

	if ( dv_spawn(myId+1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('e');
	else
		d3t_testpoint('?');

	if ( dv_spawn(DV_C0_N_EXECUTABLES-1) == dv_eid_UnconfiguredExecutable )
		d3t_testpoint('f');
	else
		d3t_testpoint('?');

	if ( dv_spawn(0) == dv_eid_ExecutableAlreadySpawned )
		d3t_testpoint('g');
	else
		d3t_testpoint('?');

	if ( dv_spawn(myId) == dv_eid_ExecutableAlreadySpawned )
		d3t_testpoint('h');
	else
		d3t_testpoint('?');

#if 0	/* ToDo: quarantine an executable */
	if ( dv_spawn(XXX) == dv_eid_ExecutableQuarantined )
		d3t_testpoint('i');
	else
		d3t_testpoint('?');
#else
	d3t_testpoint('i');
#endif


	d3t_finishtest("basics-1");
}
