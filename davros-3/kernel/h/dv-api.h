/*	dv-api.h - API header file for davros
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
#ifndef dv_api_h
#define dv_api_h	1

#if !DV_ASM

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-error.h>
#include DV_H_SYSTEMTIMER		/* For dv_readtime(), DV_MIN_SLEEP etc. */

extern void dv_nullsc(dv_machineword_t, dv_machineword_t, dv_machineword_t, dv_machineword_t);
extern void dv_exit(dv_machineword_t, dv_machineword_t);
extern dv_errorid_t dv_spawn(dv_index_t);
extern dv_dual_t dv_create_exe(const dv_execonfig_t *);
extern dv_errorid_t dv_sleep(dv_u32_t);
extern dv_errorid_t dv_sleep_until(dv_u64_t);
extern dv_errorid_t dv_suspend(void);
extern dv_errorid_t dv_resume(dv_index_t);
extern dv_index_t dv_get_exeid(void);

#endif

#endif
