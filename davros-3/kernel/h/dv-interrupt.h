/*	dv-interrupt.h - interrupt handling header file for davros
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
#ifndef dv_interrupt_h
#define dv_interrupt_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-kernel-types.h>

#if !DV_ASM

typedef void (*dv_inthandler_t)(dv_kernel_t *, unsigned);

struct dv_softvector_s
{
	dv_inthandler_t handler;
	unsigned parameter;
};

void dv_dispatch_interrupt(dv_kernel_t *, unsigned);
void dv_unknown_interrupt(dv_kernel_t *, unsigned);
void dv_unconfigured_interrupt(dv_kernel_t *, unsigned);
void dv_attach_irq(int, dv_inthandler_t, unsigned);

#endif

#endif
