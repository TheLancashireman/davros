/*	dv-coreconfig.h - core configuration for davros
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
#ifndef dv_coreconfig_h
#define dv_coreconfig_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>

struct dv_coreconfig_s
{
	dv_kernel_t *kernelvars;			/* Kernel variables structure */
	dv_stackword_t *kernelstack;		/* Stack array */
	dv_stackword_t *kernelstacktop;		/* Initial stack pointer */
	dv_executable_t *executables;		/* Array of executables on this core */
	dv_thread_t *threads;				/* Array of threads on this core */
	dv_registers_t *registers;			/* Array of register stores on this core */
	dv_eventstatus_t *events;			/* Array of event status structures on this core */
	dv_dllelement_t *dll_elements;		/* Array of dll element structures on this core */
	dv_ringbuffer_t *ringbuffers;		/* Array of ringbuffer structures on this core */
	dv_u32_t *buffers;					/* Array of ringbuffer elements this core */
	dv_page_t *pages;					/* Array of pages on this core */
	dv_mempage_t *mempage;				/* Array of management structs for pages */
	const dv_execonfig_t *idle_cfg;		/* Idle thread configuration */
	const dv_execonfig_t *init_cfg;		/* Init/main thread configuration */

	dv_index_t core_index;				/* Core index */
	dv_quantity_t n_kernelstack;		/* No of stackwords in kernel stack */
	dv_quantity_t n_executables;		/* No of executables in executables array */
	dv_quantity_t n_threads;			/* No of threads in the threads array */
	dv_quantity_t n_registers;			/* No of register stores in the registers array */
	dv_quantity_t n_events;				/* No of event status structures in the events array */
	dv_quantity_t n_dll_elements;		/* No of dll element structures in the dll_elements array */
	dv_quantity_t n_ringbuffers;		/* No of ringbuffer structures in the ringbuffer array */
	dv_quantity_t n_ringbufferwords;	/* No of words in the buffers array */
	dv_quantity_t n_pages;				/* No of pages and management structs */
};

extern const dv_coreconfig_t *const dv_coreconfigs[DV_N_CORES];

/* DV_CORELIST() creates a list of initialisers for an array with one entry per core.
 *
 * Currently up to 8 cores are supported.
*/
#if DV_N_CORES == 1
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0
#elif DV_N_CORES == 2
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1
#elif DV_N_CORES == 3
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2
#elif DV_N_CORES == 4
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3
#elif DV_N_CORES == 5
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4
#elif DV_N_CORES == 6
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5
#elif DV_N_CORES == 7
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5, c6
#elif DV_N_CORES == 8
#define DV_CORELIST(c0, c1, c2, c3, c4, c5, c6, c7)	c0, c1, c2, c3, c4, c5, c6, c7
#endif

/* dv_vector_location is the location of the hardware interrupt/exception vectors.
 * This address is used in a hardware-dependent way but can be defined independently of the hardware.
*/
extern void *const dv_vector_location;

#endif
