/*	dv-semaphore.h - semaphore structure for davros
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
#ifndef dv_semaphore_h
#define dv_semaphore_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-doublylinkedlist.h>

#if !DV_ASM

/* Semaphore protocols
*/
enum dv_semaphoreprotocol_e
{
	dv_semaphore_none = 0,			/* No mechanism; semaphore has not been configured */
	dv_semaphore_fifo,				/* Classic P/V (wait/signal) semaphore with fifo queue */
	dv_semaphore_priority,			/* Classic P/V (wait/signal) semaphore with priority queue */
	dv_semaphore_inheritance,		/* Priority inheritance protocol - always has priority queue */
	dv_semaphore_deferredceiling,	/* Priority ceiling protocol - always has priority queue */
	dv_semaphore_immediateceiling,	/* Immediate priority ceiling protocol (like OSEK/AUTOSAR) - no queue */
	dv_semaphore_nprotocols
};
typedef enum dv_semaphoreprotocol_e dv_semaphoreprotocol_t;

/* String representations for info functions.
*/
#define DV_SEMAPHOREPROTOCOLS \
	[dv_semaphore_none]				= "dv_semaphore_none",				\
	[dv_semaphore_fifo]				= "dv_semaphore_fifo",				\
	[dv_semaphore_priority]			= "dv_semaphore_priority",			\
	[dv_semaphore_deferredceiling]	= "dv_semaphore_deferredceiling",	\
	[dv_semaphore_immediateceiling]	= "dv_semaphore_immediateceiling"

/* The structure representing a semaphore.
*/
struct dv_semaphore_s
{
	dv_doublylinkedlist_t exe_queue;	/* List of executables that are waiting for the semaphore */
	dv_executable_t *owner;				/* Current owner of the semaphore */
	dv_semaphore_t *link;				/* Next in the list of semaphores that are held by the owner. */
	dv_i32_t oldprio;					/* Priority of owner before taking the semaphore */
	dv_i32_t ceiling;					/* Ceiling priority */
	dv_semaphoreprotocol_t protocol;	/* Protocol to use for this semaphore */
	dv_i32_t count;						/* Counter */
};


extern dv_errorid_t dv_wait_semimmceil(dv_kernel_t *, dv_semaphore_t *);
extern dv_errorid_t dv_signal_semimmceil(dv_kernel_t *, dv_semaphore_t *);

#endif

#endif
