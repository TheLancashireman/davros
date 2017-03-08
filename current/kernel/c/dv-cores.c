/*	dv-cores.c - core lookup tables etc. for davros
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
#include <kernel/h/dv-c0.h>
#if DV_NCORES > 1
#include <kernel/h/dv-c1.h>
#endif
#if DV_NCORES > 2
#include <kernel/h/dv-c2.h>
#endif
#if DV_NCORES > 3
#include <kernel/h/dv-c3.h>
#endif
#if DV_NCORES > 4
#include <kernel/h/dv-c4.h>
#endif
#if DV_NCORES > 5
#include <kernel/h/dv-c5.h>
#endif
#if DV_NCORES > 6
#include <kernel/h/dv-c6.h>
#endif
#if DV_NCORES > 7
#include <kernel/h/dv-c7.h>
#endif

dv_kernel_t *const dv_kernelvars[DV_NCORES] =
{	DV_CORELIST(	&dv_c0_kvars,
					&dv_c1_kvars,
					&dv_c2_kvars,
					&dv_c3_kvars,
					&dv_c4_kvars,
					&dv_c5_kvars,
					&dv_c6_kvars,
					&dv_c7_kvars)
};

dv_stackword_t *const dv_kernelstack[DV_NCORES] =
{	DV_CORELIST(	dv_c0_kernelstack,
					dv_c1_kernelstack,
					dv_c2_kernelstack,
					dv_c3_kernelstack,
					dv_c4_kernelstack,
					dv_c5_kernelstack,
					dv_c6_kernelstack,
					dv_c7_kernelstack)
};

dv_stackword_t *const dv_kernelstacktop[DV_NCORES] =
{	DV_CORELIST(	&dv_c0_kernelstack[DV_KSTACK_WORDS],
					&dv_c1_kernelstack[DV_KSTACK_WORDS],
					&dv_c2_kernelstack[DV_KSTACK_WORDS],
					&dv_c3_kernelstack[DV_KSTACK_WORDS],
					&dv_c4_kernelstack[DV_KSTACK_WORDS],
					&dv_c5_kernelstack[DV_KSTACK_WORDS],
					&dv_c6_kernelstack[DV_KSTACK_WORDS],
					&dv_c7_kernelstack[DV_KSTACK_WORDS])
};
