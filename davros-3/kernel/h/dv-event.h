/*	dv-event.h - event status structure
 *
 *	Copyright David Haworth
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
#ifndef dv_event_h
#define dv_event_h	1

#include <dv-types.h>

#if !DV_ASM

struct dv_eventstatus_s
{
	dv_u64_t pending_events;
	dv_u64_t awaited_events;
	dv_boolean_t allocated;
};

extern dv_eventstatus_t *dv_allocate_eventstatus(dv_kernel_t *kvars);
extern void dv_deallocate_eventstatus(dv_kernel_t *kvars, dv_eventstatus_t *);

#define DV_NO_EVENTS	0LL

#endif

#endif
