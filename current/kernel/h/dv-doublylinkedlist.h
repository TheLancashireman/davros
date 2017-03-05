/*	dv-doublylinkedlist.h - doubly-linked list structures for davros
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
#ifndef dv_doublylinkedlist_h
#define dv_doublylinkedlist_h	1

#include <kernel/h/dv-kernel-types.h>

#if !DV_ASM

typedef dv_int32_t dv_dllkey_t;
#define DV_DLLMINKEY	((dv_dllkey_t)0x80000000)

enum dv_dlltype_e
{
	dv_dll_priority = 1,
	dv_dll_delta
};

typedef enum dv_dlltype_e dv_dlltype_t;

/* An entry in the list.
*/
struct dv_dllelement_s
{
	dv_dllelement_t *successor;
	dv_dllelement_t *predecessor;
	dv_dllkey_t key;
	void *payload;
};

/* The list itself
 * A single entry is used to give the head and tail
 * Its successor is the first element, its predecessor is the last element.
 * The key (in a priority queue) is the smallest key value.
 * Key values for other types of queue ...?
 * It's payload is always null.
*/
struct dv_doublylinkedlist_s
{
	dv_dllelement_t headtail;
	dv_dlltype_t listtype;
};

/* Functions
*/
void dv_dllinit(dv_doublylinkedlist_t *, dv_dlltype_t);
void dv_dllinsertbeforesame(dv_doublylinkedlist_t *, dv_dllelement_t *);
void dv_dllinsertaftersame(dv_doublylinkedlist_t *, dv_dllelement_t *);

#endif

#endif
