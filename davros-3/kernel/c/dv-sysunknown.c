/*	dv-sysunknown.c - unknown/unimplemented system call for davros
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-syscall.h>

DV_COVDEF(sys_unknown);

/* dv_sys_unknown() - report an error for an unknown/unimplemented system call
 *
 * This function handles unknown and unimplemented system calls in the kernel.
*/
void dv_sys_unknown(dv_kernel_t *unused_kvars, dv_index_t unused_sci)
{
}

/* man-page-generation - to be defined
*/
