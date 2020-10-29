/* d3t-finishtest.c - finish a single test
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
#include <dv-projectconfig.h>
#include <d3t.h>
#include <dv-string.h>

/* d3t_finishtest() - finish a test case
 *
 * Print the actual sequence.
 * Compate actual with expected.
 * Print a message with passed/fail.
*/
void d3t_finishtest(char *name)
{
	char *result;
	dv_kprintf("! got    : %s\n", d3t_testsequence);

	if ( d3t_expected == DV_NULL )
		result = "no expected sequence";
	else if ( dv_strcmp(d3t_testsequence, d3t_expected) == 0 )
		result = "passed";
	else
		result = "failed";
    dv_kprintf("! finish : %s %s\n", name, result);
}
