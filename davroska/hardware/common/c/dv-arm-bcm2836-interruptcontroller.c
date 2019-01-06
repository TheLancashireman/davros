/*	dv-arm-bcm2836-interruptcontroller.c - BCM2836 interrupt controller functions for davroska
 *
 *	Copyright 2019 David Haworth
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
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-arm-bcm2836-interruptcontroller.h>
#include <dv-arm-bcm2835-interruptcontroller.h>
#include <davroska-inline.h>

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

const dv_bcm2836_irq_t dv_bcm2836_irq_list[dv_n_bcm2836_iid - dv_n_bcm2835_iid] =
{
	[dv_iid_cntps - dv_n_bcm2835_iid]	= {	DV_BCM2836_SRC_CNTPS	},
	[dv_iid_cntpns - dv_n_bcm2835_iid]	= {	DV_BCM2836_SRC_CNTPNS	},
	[dv_iid_cnthp - dv_n_bcm2835_iid]	= {	DV_BCM2836_SRC_CNTHP	},
	[dv_iid_cntv - dv_n_bcm2835_iid]	= {	DV_BCM2836_SRC_CNTV		},
	[dv_iid_mb0 - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_MB0		},
	[dv_iid_mb1 - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_MB1		},
	[dv_iid_mb2 - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_MB2		},
	[dv_iid_mb3 - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_MB3		},
	[dv_iid_pmu - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_PMU		},
	[dv_iid_axi - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_AXI		},
	[dv_iid_lt - dv_n_bcm2835_iid]		= {	DV_BCM2836_SRC_LT		},
#if 0
	[dv_iid_gpu - dv_n_bcm2835_iid]		= { DV_BCM2836_SRC_GPU		},
	[dv_iid_periph - dv_n_bcm2835_iid]	= { DV_BCM2836_SRC_PERIPH	},
#endif
};

#ifndef DV_CFG_BCM2836_VECTOR_START
#define DV_CFG_BCM2836_VECTOR_START	dv_n_bcm2835_iid
#endif

void dv_bcm2836_interrupt_handler(void)
{
	dv_u32_t pend;
	pend = dv_bcm2836_periphs.irq_source[DV_TARGET_CORE];
	if ( pend & DV_BCM2836_SRC_GPU )
	{
		pend &= ~DV_BCM2836_SRC_GPU;
		dv_bcm2835_interrupt_handler();
	}

	/* Loop over all BCM2836 interrupt sources. Clear and handle the configured sources.
	*/
	for ( int i = dv_n_bcm2835_iid; i < dv_n_bcm2836_iid; i++ )
	{
		dv_u32_t mask = dv_bcm2836_irq_list[i - dv_n_bcm2835_iid].mask;
		if ( pend & mask )
		{
			pend &= ~mask;
			dv_softvector(i);
		}
	}

	/* Report unknown interrupts.
	 *
	 * Is there a simple way to turn them off?
	*/
	if ( pend != 0)
	{
		dv_printf("dv_bcm2836_interrupt_handler() - core %d: unknown interrupts: 0x%08x\n",
					DV_TARGET_CORE, pend);
	}
}
