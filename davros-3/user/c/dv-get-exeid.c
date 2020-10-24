/*	dv-get-exeid.c - dv_get_exeid()
 *
 *	Copyright 2017 David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>

/* dv_get_exeid() - return the ID of the current executable (if there is one)
*/
dv_index_t dv_get_exeid(void)
{
	dv_kernel_t *kvars = dv_get_kvars();

	if ( kvars == DV_NULL ||
		 kvars->current_thread == DV_NULL ||
		 kvars->current_thread->executable == DV_NULL )
		return -1;
	else
		return kvars->current_thread->executable->id;
}
