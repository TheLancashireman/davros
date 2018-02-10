/*	dv-dllinserttime.c - insert an element into a time queue
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
#include <kernel/h/dv-doublylinkedlist.h>

#define DV_COVNAME	dllinserttime
DV_COVDEF(dllinserttime);

/* dv_dllinserttime() - insert an element into a time queue.
 *
 * The element is inserted after all others of the same or earlier time
 * but before those of later time.
 *
 * Returns TRUE if the element was inserted at the head of the list.
*/
dv_boolean_t dv_dllinserttime(dv_doublylinkedlist_t *list, dv_dllelement_t *elem)
{
	dv_dllelement_t *e = list->headtail.successor;
	dv_fcov(0);

	while ( dv_ccov(1, 2, (elem->key.u64_key >= e->key.u64_key)) )
	{
		e = e->successor;
	}

	elem->predecessor = e->predecessor;
	elem->successor = e;
	e->predecessor->successor = elem;
	e->predecessor = elem;

	return (elem->predecessor == &list->headtail);
}

/* man-page-generation - to be defined
*/
