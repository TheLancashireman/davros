/*	dv-start.c - start davros
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-coverage.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(start);

/* dv_start() - start Davros on one core
*/
void dv_start(dv_identity_t ci)
{
	dv_kernel_t *kvars = dv_kernelvars[ci];

	kvars->current_thread = DV_NULL;
	kvars->kernel_sp = dv_kernelstacktop[ci];
	kvars->core_index = ci;
	dv_dllinit(&kvars->thread_queue, dv_dll_priority);
	kvars->panic_reason[0] = kvars->panic_reason[1] = dv_panic_none;

	dv_panic(dv_panic_unimplemented, "dv_start", "function isn't complete");
}

/* man-page-generation - to be defined
*/
