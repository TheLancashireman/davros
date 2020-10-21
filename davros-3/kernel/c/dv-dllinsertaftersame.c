/*	dv-dllinsertaftersame.c - insert an element into a priority queue, after elements of the same priority
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-doublylinkedlist.h>

#define DV_COVNAME	dllinsertaftersame
DV_COVDEF(dllinsertaftersame);

/* dv_dllinsertaftersame_elem() - insert an element into a priority queue relative to a given element.
 *
 * The element is inserted after all others of the same or higher priority
 * but before those of lower priority.
*/
void dv_dllinsertaftersame_elem(dv_dllelement_t *start, dv_dllelement_t *elem)
{
	dv_fcov(0);

	dv_dllelement_t *e = start;

	while ( dv_ccov(1, 2, (elem->key.i32_key <= e->key.i32_key)) )
	{
		e = e->successor;
	}

	dv_dllinsertbefore(e, elem);
}
