/*	dv-kernel.h - kernel variables for davros
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
#ifndef dv_kernel_h
#define dv_kernel_h	1

#include <kernel/include/dv-kconfig.h>
#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-kernel-types.h>
#include <kernel/include/dv-doublylinkedlist.h>

#if !DV_ASM

struct dv_kernel_s
{
	dv_doublylinkedlist_t threadqueue;
};

#endif

#endif
