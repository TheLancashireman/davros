/*	dv-kernel-types.h - kernel data types for davros
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
#ifndef dv_kernel_types_h
#define dv_kernel_types_h	1

#if !DV_ASM
typedef struct dv_coverage_s dv_coverage_t;
typedef struct dv_coreconfig_s dv_coreconfig_t;
typedef struct dv_dllelement_s dv_dllelement_t;
typedef struct dv_doublylinkedlist_s dv_doublylinkedlist_t;
typedef struct dv_eventstatus_s dv_eventstatus_t;
typedef struct dv_executable_s dv_executable_t;
typedef struct dv_execonfig_s dv_execonfig_t;
typedef struct dv_kernel_s dv_kernel_t;
typedef struct dv_registers_s dv_registers_t;
typedef struct dv_thread_s dv_thread_t;
typedef struct dv_lock_s dv_lock_t;
typedef struct dv_page_s dv_page_t;
typedef struct dv_mempage_s dv_mempage_t;
typedef struct dv_syscall_s dv_syscall_t;
typedef struct dv_softvector_s dv_softvector_t;
typedef struct dv_ringbuffer_s dv_ringbuffer_t;
typedef struct dv_job_s dv_job_t;
typedef struct dv_kobjallocator_s dv_kobjallocator_t;

typedef void (*dv_function_t)(void);
#endif

#endif
