/* d3t-testpoint.c - record a test point
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

void d3t_testpoint(char tp)
{
#if 1
	if ( d3t_seqcount >= D3T_MAX_SEQ )
		d3t_abort("test sequence overflow", d3t_seqcount);

	d3t_testsequence[d3t_seqcount++] = tp;
#else
	dv_kputc(tp);
#endif
}
