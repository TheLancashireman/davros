/*	dv-syscall.h - header file for davros systrem call handling
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
#ifndef dv_syscall_h
#define dv_syscall_h	1

#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>

#define DV_SC_exit				0
#define DV_SC_yield				1
#define DV_SC_activate			2
#define DV_SC_activate_async	3
#define DV_SC_chain				4
#define DV_SC_acquire			5
#define DV_SC_drop				6
#define DV_SC_event_wait		8
#define DV_SC_event_wgc			9
#define DV_SC_event_wgc_all		10
#define DV_SC_event_clear		11
#define DV_SC_event_set			12
#define DV_SC_event_set_async	13
#define DV_SC_event_get			14

#define DV_N_SYSCALL			15

#if !DV_ASM

enum dv_sc_e
{
	dv_sc_exit				= DV_SC_exit,
	dv_sc_yield				= DV_SC_yield,
	dv_sc_activate			= DV_SC_activate,
	dv_sc_activate_async	= DV_SC_activate_async,
	dv_sc_chain				= DV_SC_chain,
	dv_sc_acquire			= DV_SC_acquire,
	dv_sc_drop				= DV_SC_drop,
	dv_sc_event_wait		= DV_SC_event_wait,
	dv_sc_event_wgc			= DV_SC_event_wgc,
	dv_sc_event_wgc_all		= DV_SC_event_wgc_all,
	dv_sc_event_clear		= DV_SC_event_clear,
	dv_sc_event_set			= DV_SC_event_set,
	dv_sc_event_set_async	= DV_SC_event_set_async,
	dv_sc_event_get			= DV_SC_event_get,
	dv_sc_n_syscall
};

typedef enum dv_sc_t dv_sc_e;

void dv_syscall(dv_kernel_t *, dv_machineword_t) __attribute__((noreturn));

typedef void (*dv_sc_function_t)(dv_kernel_t *);

extern dv_sc_function_t const dv_syscalltable[DV_N_SYSCALL+1];

void dv_sys_unknown(dv_kernel_t *);
void dv_sys_exit(dv_kernel_t *);
void dv_sys_yield(dv_kernel_t *);
void dv_sys_activate(dv_kernel_t *);
void dv_sys_activate_async(dv_kernel_t *);
void dv_sys_chain(dv_kernel_t *);
void dv_sys_acquire(dv_kernel_t *);
void dv_sys_drop(dv_kernel_t *);
void dv_sys_event_wait(dv_kernel_t *);
void dv_sys_event_wgc(dv_kernel_t *);
void dv_sys_event_wgc_all(dv_kernel_t *);
void dv_sys_event_clear(dv_kernel_t *);
void dv_sys_event_set(dv_kernel_t *);
void dv_sys_event_set_async(dv_kernel_t *);
void dv_sys_event_get(dv_kernel_t *);

#endif

#endif
