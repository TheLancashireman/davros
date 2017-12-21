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

#define DV_SC_null				0
#define DV_SC_exit				1
#define DV_SC_yield				2
#define DV_SC_create_exe		3
#define DV_SC_spawn				4		/* Must be even */
#define DV_SC_spawn_async		5		/* Must be DV_SC_spawn+1 */
#define DV_SC_chain				6
#define DV_SC_acquire			7
#define DV_SC_drop				8
#define DV_SC_event_wait		9
#define DV_SC_event_wgc			10
#define DV_SC_event_wgc_all		11
#define DV_SC_event_set			12		/* Must be even */
#define DV_SC_event_set_async	13		/* Must be DV_SC_event_set+1 */
#define DV_SC_event_get			14
#define DV_SC_event_clear		15

#define DV_N_SYSCALL			16

#if !DV_ASM

enum dv_sc_e
{
	dv_sc_null				= DV_SC_null,
	dv_sc_exit				= DV_SC_exit,
	dv_sc_yield				= DV_SC_yield,
	dv_sc_create_exe		= DV_SC_create_exe,
	dv_sc_spawn				= DV_SC_spawn,
	dv_sc_spawn_async		= DV_SC_spawn_async,
	dv_sc_chain				= DV_SC_chain,
	dv_sc_acquire			= DV_SC_acquire,
	dv_sc_drop				= DV_SC_drop,
	dv_sc_event_wait		= DV_SC_event_wait,
	dv_sc_event_wgc			= DV_SC_event_wgc,
	dv_sc_event_wgc_all		= DV_SC_event_wgc_all,
	dv_sc_event_set			= DV_SC_event_set,
	dv_sc_event_set_async	= DV_SC_event_set_async,
	dv_sc_event_get			= DV_SC_event_get,
	dv_sc_event_clear		= DV_SC_event_clear,
	dv_sc_n_syscall
};

#define DV_SYSCALLTABLE \
/*  0 */	{ dv_sys_null,			"dv_sys_null"			},	\
/*  1 */	{ dv_sys_exit,			"dv_sys_exit"			},	\
/*  2 */	{ dv_sys_yield,			"dv_sys_yield"			},	\
/*  3 */	{ dv_sys_create_exe,	"dv_sys_create_exe"		},	\
/*  4 */	{ dv_sys_spawn,			"dv_sys_spawn"			},	\
/*  5 */	{ dv_sys_spawn,			"dv_sys_spawn"			},	\
/*  6 */	{ dv_sys_chain,			"dv_sys_chain"			},	\
/*  7 */	{ dv_sys_acquire,		"dv_sys_acquire"		},	\
/*  8 */	{ dv_sys_drop,			"dv_sys_drop"			},	\
/*  9 */	{ dv_sys_event_wait,	"dv_sys_event_wait"		},	\
/* 10 */	{ dv_sys_event_wgc,		"dv_sys_event_wgc"		},	\
/* 11 */	{ dv_sys_event_wgc_all,	"dv_sys_event_wgc_all"	},	\
/* 12 */	{ dv_sys_event_set,		"dv_sys_event_set"		},	\
/* 13 */	{ dv_sys_event_set,		"dv_sys_event_set"		},	\
/* 14 */	{ dv_sys_event_get,		"dv_sys_event_get"		},	\
/* 15 */	{ dv_sys_event_clear,	"dv_sys_event_clear"	},	\
/* 16 */	{ dv_sys_unknown,		"dv_sys_unknown"		}

typedef enum dv_sc_t dv_sc_e;

void dv_syscall(dv_kernel_t *, dv_machineword_t) __attribute__((noreturn));

typedef void (*dv_sc_function_t)(dv_kernel_t *, dv_index_t);

struct dv_syscall_s
{
	dv_sc_function_t function;
	const char *name;
};

extern const dv_syscall_t dv_syscalltable[DV_N_SYSCALL+1];

void dv_sys_null(dv_kernel_t *, dv_index_t);
void dv_sys_exit(dv_kernel_t *, dv_index_t);
void dv_sys_yield(dv_kernel_t *, dv_index_t);
void dv_sys_create_exe(dv_kernel_t *, dv_index_t);
void dv_sys_spawn(dv_kernel_t *, dv_index_t);
void dv_sys_chain(dv_kernel_t *, dv_index_t);
void dv_sys_acquire(dv_kernel_t *, dv_index_t);
void dv_sys_drop(dv_kernel_t *, dv_index_t);
#if 0
void dv_sys_event_wait(dv_kernel_t *, dv_index_t);
void dv_sys_event_wgc(dv_kernel_t *, dv_index_t);
void dv_sys_event_wgc_all(dv_kernel_t *, dv_index_t);
void dv_sys_event_set(dv_kernel_t *, dv_index_t);
void dv_sys_event_get(dv_kernel_t *, dv_index_t);
void dv_sys_event_clear(dv_kernel_t *, dv_index_t);
#else
#define dv_sys_event_wait		dv_sys_unknown
#define dv_sys_event_wgc		dv_sys_unknown
#define dv_sys_event_wgc_all	dv_sys_unknown
#define dv_sys_event_set		dv_sys_unknown
#define dv_sys_event_set_async	dv_sys_unknown
#define dv_sys_event_get		dv_sys_unknown
#define dv_sys_event_clear		dv_sys_unknown
#endif

void dv_sys_unknown(dv_kernel_t *, dv_index_t);

#endif

#endif
