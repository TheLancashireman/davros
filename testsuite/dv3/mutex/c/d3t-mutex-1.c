/* d3t-mutex-1.c - davros-3 test suite : mutex test 1
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

/* d3t_mutex_1() - error returns from dv_acquire() and dv_relinquish()
*/
void d3t_mutex_1(void)
{
	d3t_starttest("mutex-1 - errors from dv_acquire() and dv_relinquish()");
	d3t_expect("a");
	d3t_testpoint('a');
	d3t_finishtest("mutex-1");
}
