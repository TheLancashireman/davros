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

#define DV_DLLMINI32KEY	(-1073741824-1073741824)

typedef union dv_dllkey_u dv_dllkey_t;

union dv_dllkey_u
{
	dv_i32_t i32_key;
	dv_u64_t u64_key;
};

enum dv_dllptype_e
{
	dv_dll_free = 0,
	dv_dll_thread = 1,
	dv_dll_exe
};

enum dv_dlltype_e
{
	dv_dll_priority = 1,
	dv_dll_time
};

typedef enum dv_dllptype_e dv_dllptype_t;
typedef enum dv_dlltype_e dv_dlltype_t;

/* An entry in the list.
*/
struct dv_dllelement_s
{
	dv_dllelement_t *successor;
	dv_dllelement_t *predecessor;
	dv_dllkey_t key;
	dv_dllptype_t payload_type;
	void *payload;
};

/* The list itself
 * A single entry is used to give the head and tail
 * Its successor is the first element, its predecessor is the last element.
 * The key for a priority queue is the smallest key value.
 * The key for a time queue is the largest time.
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
dv_boolean_t dv_dllinserttime(dv_doublylinkedlist_t *, dv_dllelement_t *);

static inline void dv_dllremove(dv_dllelement_t *elem)
{
	elem->successor->predecessor = elem->predecessor;
	elem->predecessor->successor = elem->successor;
	elem->successor = elem->predecessor = DV_NULL;
}

static inline dv_boolean_t dv_dllisempty(dv_doublylinkedlist_t *list)
{
	return (list->headtail.successor == list->headtail.predecessor);
}

dv_dllelement_t *dv_allocate_dllelement(dv_kernel_t *kvars);

#endif

#endif
