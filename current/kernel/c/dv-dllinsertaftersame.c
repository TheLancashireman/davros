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

#include <kernel/include/dv-kconfig.h>
#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-doublylinkedlist.h>
#include <kernel/include/dv-coverage.h>

#define DV_COVNAME	dllinsertaftersame
DV_COVDEF(dllinsertaftersame);

/* dv_dllinsertaftersame() - insert an element into a priority queue.
 *
 * The element is inserted after all others of the same or higher priority
 * but before those of lower priority.
*/
void dv_dllinsertaftersame(dv_doublylinkedlist_t *list, dv_dllelement_t *elem)
{
	dv_dllelement_t *e = list->headtail.successor;
	dv_fcov(0);

	while ( dv_ccov(1, 2, (elem->key <= e->key)) )
	{
		e = e->successor;
	}

	elem->predecessor = e->predecessor;
	elem->successor = e;
	e->predecessor->successor = elem;
	e->predecessor = elem;
}

/* man-page-generation - to be defined
*/
