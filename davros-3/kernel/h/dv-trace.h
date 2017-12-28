/*	dv-trace.h - tracing for davros
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
#ifndef dv_trace_h
#define dv_trace_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-thread.h>

#if DV_TRACE

/* The following must be provided by the project.
*/
void dv_trace_threadstate(dv_thread_t *thread, dv_threadstate_t newstate);
void dv_trace_api(dv_thread_t *thread, dv_index_t sci, const dv_syscall_t *sc);
void dv_trace_dumpregs(const char *str, const dv_registers_t *r);
void dv_trace_dumpcpuregs(void);

#else

static inline void dv_trace_threadstate(dv_thread_t *unused_thread, dv_threadstate_t unused_newstate)
{
}

static inline void dv_trace_api(dv_thread_t *unused_thread, dv_index_t unused_sci, const dv_syscall_t *unused_sc)
{
}

static inline void dv_trace_dumpregs(const char *unused_str, const dv_registers_t *unused_r)
{
}

static inline void dv_trace_dumpcpuregs(void)
{
}

#endif


#endif
