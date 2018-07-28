/*	dv-arm-bcm2836-interruptcontroller.c - BCM2836 interrupt controller functions
 *
 *	Copyright 2018 David Haworth
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
#include <kernel/h/dv-interrupt.h>
#include <devices/h/dv-arm-bcm2835-interruptcontroller.h>
#include <devices/h/dv-arm-bcm2836.h>

/* dv_bcm2836_interrupt_handler() - interrupt handler (BCM2836 version)
 *
 * The BCM2836 interrupt controller is a very simple unit. There is no vectoring, and
 * there are no priority levels.
 *
 * There's a pending register per core.
 * The DV_BCM2836_SRC_PERIPH interrupts are supposedly not connected so they are omitted from the list.
 * The DV_BCM2836_SRC_GPU interrupt is passed on to the dv_bcm2835_interrupt_handler()
 *
*/

const dv_bcm2836_irq_t dv_bcm2836_irq_list[dv_n_bcm2836_iid] =
{
	[dv_iid_cntps]	= {	DV_BCM2836_SRC_CNTPS	},
	[dv_iid_cntpns]	= {	DV_BCM2836_SRC_CNTPNS	},
	[dv_iid_cnthp]	= {	DV_BCM2836_SRC_CNTHP	},
	[dv_iid_cntv]	= {	DV_BCM2836_SRC_CNTV		},
	[dv_iid_mb0]	= {	DV_BCM2836_SRC_MB0		},
	[dv_iid_mb1]	= {	DV_BCM2836_SRC_MB1		},
	[dv_iid_mb2]	= {	DV_BCM2836_SRC_MB2		},
	[dv_iid_mb3]	= {	DV_BCM2836_SRC_MB3		},
	[dv_iid_pmu]	= {	DV_BCM2836_SRC_PMU		},
	[dv_iid_axi]	= {	DV_BCM2836_SRC_AXI		},
	[dv_iid_lt]		= {	DV_BCM2836_SRC_LT		},
#if 0
	[dv_iid_gpu]	= { DV_BCM2836_SRC_GPU		},
	[dv_iid_periph]	= { DV_BCM2836_SRC_PERIPH	},
#endif
};

#ifndef DV_CFG_BCM2836_VECTOR_START
#define DV_CFG_BCM2836_VECTOR_START	dv_n_bcm2835_iid
#endif

void dv_bcm2836_interrupt_handler(dv_kernel_t *kvars)
{
	dv_u32_t pend = dv_bcm2836_periphs.irq_source[kvars->core_index];
	int i, vecidx;
	dv_u32_t mask;

	while ( pend != 0 )
	{
		if ( pend & DV_BCM2836_SRC_GPU )
		{
			pend &= DV_BCM2836_SRC_GPU;
			dv_bcm2835_interrupt_handler(kvars);
		}

		/* Loop over all interrupt sources. Clear and handle the configured sources.
		*/
		for ( i = 0; i < dv_n_bcm2836_iid; i++ )
		{
			mask = dv_bcm2836_irq_list[i].mask;
			if ( pend & mask )
			{
				pend &= ~mask;
				vecidx = DV_CFG_BCM2836_VECTOR_START+i;
				(*dv_softvector[vecidx].handler)(kvars, dv_softvector[vecidx].parameter);
			}
		}

		/* Report unknown interrupts.
		 *
		 * Is there a simple way to turn them off?
		*/
		if ( pend != 0)
		{
			dv_kprintf("dv_bcm2836_interrupt_handler() - core %d: unknown interrupts: 0x%08x\n",
						kvars->core_index, pend);
		}

		pend = dv_bcm2836_periphs.irq_source[kvars->core_index];
	}
}
