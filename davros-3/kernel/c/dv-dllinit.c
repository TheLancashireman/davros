/*	dv-dllinit.c - initialise a doubly linked list
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(dllinit);

/* dv_dllinit() - initialise a doubly-linked list
*/
void dv_dllinit(dv_doublylinkedlist_t *list, dv_dlltype_t type)
{
	dv_fcov(0);

	list->listtype = type;
	list->headtail.predecessor = &list->headtail;
	list->headtail.successor = &list->headtail;
	list->headtail.payload = DV_NULL;

	switch ( type )
	{
	case dv_dll_priority:
		dv_fcov(1);
		list->headtail.key.i32_key = DV_DLLMINI32KEY;
		break;

	case dv_dll_time:
		dv_fcov(2);
		list->headtail.key.u64_key = 0xffffffffffffffff;
		break;

	default:		/* Unreachable! */
		dv_fcov(3);
		dv_panic(dv_panic_unknownqueuetype, "dv_dllinit", "Unknown queue type");
		break;
	}
}

/* man-page-generation - to be defined
*/
