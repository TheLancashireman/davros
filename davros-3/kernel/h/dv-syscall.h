/*	dv-syscall.h - header file for davros systrem call handling
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
#ifndef dv_syscall_h
#define dv_syscall_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>

#define DV_SC_null				0
#define DV_SC_exit				1
#define DV_SC_yield				2
#define DV_SC_create_exe		3
#define DV_SC_destroy_exe		4
#define DV_SC_spawn				5
#define DV_SC_chain				6
#define DV_SC_terminate			7
#define DV_SC_sleep				8
#define DV_SC_sleep_until		9
#define DV_SC_suspend			10
#define DV_SC_resume			11

#define DV_SC_acquire			XX
#define DV_SC_drop				XX
#define DV_SC_event_wait		XX
#define DV_SC_event_wgc			XX
#define DV_SC_event_wgc_all		XX
#define DV_SC_event_set			XX
#define DV_SC_event_get			XX
#define DV_SC_event_clear		XX

#define DV_N_SYSCALL			12

#if !DV_ASM

typedef enum dv_sc_e
{
	dv_sc_null				= DV_SC_null,
	dv_sc_exit				= DV_SC_exit,
	dv_sc_yield				= DV_SC_yield,
	dv_sc_create_exe		= DV_SC_create_exe,
	dv_sc_destroy_exe		= DV_SC_destroy_exe,
	dv_sc_spawn				= DV_SC_spawn,
	dv_sc_chain				= DV_SC_chain,
	dv_sc_terminate			= DV_SC_terminate,
	dv_sc_sleep				= DV_SC_sleep,
	dv_sc_sleep_until		= DV_SC_sleep_until,
	dv_sc_suspend			= DV_SC_suspend,
	dv_sc_resume			= DV_SC_resume,

	dv_sc_unknown			= DV_N_SYSCALL
} dv_sc_t;

#if 0
	dv_sc_acquire			= DV_SC_acquire,
	dv_sc_drop				= DV_SC_drop,
	dv_sc_event_wait		= DV_SC_event_wait,
	dv_sc_event_wgc			= DV_SC_event_wgc,
	dv_sc_event_wgc_all		= DV_SC_event_wgc_all,
	dv_sc_event_set			= DV_SC_event_set,
	dv_sc_event_get			= DV_SC_event_get,
	dv_sc_event_clear		= DV_SC_event_clear,
#endif

extern void dv_syscall(dv_kernel_t *, dv_machineword_t) __attribute__((noreturn));

typedef void (*dv_sc_function_t)(dv_kernel_t *, dv_index_t);

struct dv_syscall_s
{
	dv_sc_function_t function;
	const char *name;
};

extern const dv_syscall_t dv_syscalltable[DV_N_SYSCALL+1];

extern void dv_sys_null(dv_kernel_t *, dv_index_t);
extern void dv_sys_exit(dv_kernel_t *, dv_index_t);
extern void dv_sys_yield(dv_kernel_t *, dv_index_t);
extern void dv_sys_create_exe(dv_kernel_t *, dv_index_t);
extern void dv_sys_destroy_exe(dv_kernel_t *, dv_index_t);
extern void dv_sys_spawn(dv_kernel_t *, dv_index_t);
extern void dv_sys_chain(dv_kernel_t *, dv_index_t);
extern void dv_sys_terminate(dv_kernel_t *, dv_index_t);
extern void dv_sys_sleep(dv_kernel_t *, dv_index_t);
extern void dv_sys_sleep_until(dv_kernel_t *, dv_index_t);
extern void dv_sys_suspend(dv_kernel_t *, dv_index_t);
extern void dv_sys_resume(dv_kernel_t *, dv_index_t);

extern void dv_sys_unknown(dv_kernel_t *, dv_index_t);

#if 0
extern void dv_sys_acquire(dv_kernel_t *, dv_index_t);
extern void dv_sys_drop(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_wait(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_wgc(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_wgc_all(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_set(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_get(dv_kernel_t *, dv_index_t);
extern void dv_sys_event_clear(dv_kernel_t *, dv_index_t);
#endif

#endif

#endif
