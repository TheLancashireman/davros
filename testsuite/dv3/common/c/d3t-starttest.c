/* d3t-starttest.c - start a single test
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

/* d3t_starttest() - start a new test
 *
 * Print a message, clear out the expected and actual sequence variables.
*/
void d3t_starttest(char *name)
{
    dv_kprintf("! start  : %s\n", name);

	for ( int i = 0; i <= D3T_MAX_SEQ; i++ )
		d3t_testsequence[i] = 0;

	d3t_seqcount = 0;
	d3t_expected = DV_NULL;
}
