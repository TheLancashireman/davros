/*  dv-arm-bcm2836-interruptcontroller.h - interrupts on bcm2836 (raspberry pi2 and 3)
 *
 *  Copyright 2019 David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_arm_bcm2836_interruptcontroller_h
#define dv_arm_bcm2836_interruptcontroller_h	1

#include <dv-devices.h>
#include <dv-arm-bcm2836.h>
#include <dv-arm-bcm2835-interruptcontroller.h>

/* Interrupt identifiers
 *
 * ToDo: find a way of enabling/disabling individual interrupt sources.
*/
typedef enum dv_bcm2836_softvector_e
{
	dv_iid_cntps = dv_n_bcm2835_iid,
	dv_iid_cntpns,
	dv_iid_cnthp,
	dv_iid_cntv,
	dv_iid_mb0,
	dv_iid_mb1,
	dv_iid_mb2,
	dv_iid_mb3,
	dv_iid_pmu,
	dv_iid_axi,
	dv_iid_lt,
#if 0
	dv_iid_gpu,
	dv_iid_periph,
#endif
	dv_n_bcm2836_iid				/* Must be last */
} dv_bcm2836_softvector_t;

typedef struct dv_bcm2836_irq_s
{
	dv_u32_t mask;
} dv_bcm2836_irq_t;

extern const dv_bcm2836_irq_t dv_bcm2836_irq_list[dv_n_bcm2836_iid - dv_n_bcm2835_iid];

/* Interrupt handler function
*/
#if DV_DAVROSKA

void dv_bcm2836_interrupt_handler(void);

#else

void dv_bcm2836_interrupt_handler(dv_kernel_t *kvars);

#endif

#endif
