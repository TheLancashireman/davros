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
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-coreconfig.h>
#include <kernel/h/dv-c0.h>
#if DV_N_CORES > 1
#include <kernel/h/dv-c1.h>
#endif
#if DV_N_CORES > 2
#include <kernel/h/dv-c2.h>
#endif
#if DV_N_CORES > 3
#include <kernel/h/dv-c3.h>
#endif
#if DV_N_CORES > 4
#include <kernel/h/dv-c4.h>
#endif
#if DV_N_CORES > 5
#include <kernel/h/dv-c5.h>
#endif
#if DV_N_CORES > 6
#include <kernel/h/dv-c6.h>
#endif
#if DV_N_CORES > 7
#include <kernel/h/dv-c7.h>
#endif

const dv_coreconfig_t *const dv_coreconfigs[DV_N_CORES] =
{	DV_CORELIST(	&dv_c0_coreconfig,
					&dv_c1_coreconfig,
					&dv_c2_coreconfig,
					&dv_c3_coreconfig,
					&dv_c4_coreconfig,
					&dv_c5_coreconfig,
					&dv_c6_coreconfig,
					&dv_c7_coreconfig)
};

void *const dv_vector_location = (void *)DV_VECTOR_LOCATION;
