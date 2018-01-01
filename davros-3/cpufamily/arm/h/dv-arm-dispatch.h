/*	dv-arm-dispatch.h - ARM dispatcher functions for davros
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
#ifndef dv_arm_dispatch_h
#define dv_arm_dispatch_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-thread.h>

#if !DV_ASM

/* Low-level thread dispatcher (assembly language)
*/
void dv_arm_return_to_thread(dv_registers_t *regs) __attribute__((noreturn));

/* Hardware-independent interface to low-level thread dispatcher.
*/
static inline void dv_return_to_thread(dv_kernel_t *unused_kv, dv_thread_t *incoming) __attribute__((noreturn));
static inline void dv_return_to_thread(dv_kernel_t *unused_kv, dv_thread_t *incoming)
{
	/* ToDo: interrupt lock level */
	dv_arm_return_to_thread(incoming->regs);
}

#endif

#endif
