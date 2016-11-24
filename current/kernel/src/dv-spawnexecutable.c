/*	dv-spawnexecutable.c - spawn an instance of an executable object
 *
 *	Copyright 2015 David Haworth
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
#include <kernel/include/dv-kconfig.h>
#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-kernel-types.h>
#include <kernel/include/dv-kernel.h>
#include <kernel/include/dv-executable.h>
#include <kernel/include/dv-thread.h>
#include <kernel/include/dv-event.h>
#include <kernel/include/dv-coverage.h>
#include <user/include/dv-error.h>

DV_COVDEF(spawnexecutable);

/* dv_spawnexecutable() - spawn an instance of an executable
*/
dv_errorid_t dv_spawnexecutable(dv_kernel_t *kvars, dv_executable_t *executable)
{
	dv_errorid_t ecode = dv_eid_UnknownError;

	if ( executable->enabled )
	{
		if ( executable->n_instances < executable->maxinstances )
		{
			executable->n_instances++;

			if ( executable->events != DV_NULL )
			{
				executable->events->pending_events = DV_NO_EVENTS;
				executable->events->awaited_events = DV_NO_EVENTS;
			}

			ecode = dv_spawnexecutableinthread(&kvars->thread_queue, executable->thread, executable);
		}
		else
		{
			ecode = dv_eid_MaxInstancesExceeded;
		}
	}
	else
	{
		ecode = dv_eid_ExecutableQuarantined;
	}

	return ecode;
}

/* man-page-generation - to be defined
*/
