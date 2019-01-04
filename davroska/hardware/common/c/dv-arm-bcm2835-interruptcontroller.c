/*	dv-arm-bcm2835-interruptcontroller.c - BCM2835 interrupt controller functions
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
#include <dv-config.h>
#include <davroska.h>
#include <dv-arm-bcm2835-interruptcontroller.h>
#include <davroska-inline.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif

/* dv_bcm2835_irq_list[] is a table of all the interrupt sources that can be used.
 * The index in this list gives the vector number of the IRQ.
 * The list must be ordered so that the register numbers are clustered in order 0,1,2.
*/
const dv_bcm2835_irq_t dv_bcm2835_irq_list[dv_n_bcm2835_iid+1] =
{
	[dv_iid_syst_cm1]	= {	0,	DV_INT_SYST_CM1		},
	[dv_iid_syst_cm3]	= {	0,	DV_INT_SYST_CM3		},
	[dv_iid_aux]		= {	0,	DV_INT_AUX			},
	[dv_iid_i2cspi_slv]	= {	1,	DV_INT_I2CSPI_SLV	},
	[dv_iid_pwa0]		= {	1,	DV_INT_PWA0			},
	[dv_iid_pwa1]		= {	1,	DV_INT_PWA1			},
	[dv_iid_smi]		= {	1,	DV_INT_SMI			},
	[dv_iid_gpio0]		= {	1,	DV_INT_GPIO0		},
	[dv_iid_gpio1]		= {	1,	DV_INT_GPIO1		},
	[dv_iid_gpio2]		= {	1,	DV_INT_GPIO2		},
	[dv_iid_gpio3]		= {	1,	DV_INT_GPIO3		},
	[dv_iid_i2c]		= {	1,	DV_INT_I2C			},
	[dv_iid_spi]		= {	1,	DV_INT_SPI			},
	[dv_iid_pcm]		= {	1,	DV_INT_PCM			},
	[dv_iid_uart]		= {	1,	DV_INT_UART			},
	[dv_iid_timer]		= {	2,	DV_INT_TIMER		},
	[dv_iid_mailbox]	= {	2,	DV_INT_MAILBOX		},
	[dv_iid_doorbell0]	= {	2,	DV_INT_DOORBELL0	},
	[dv_iid_doorbell1]	= {	2,	DV_INT_DOORBELL1	},
	[dv_iid_gpu0halt]	= {	2,	DV_INT_GPU0HALT		},
	[dv_iid_gpu1halt]	= {	2,	DV_INT_GPU1HALT		},
	[dv_iid_illegal0]	= {	2,	DV_INT_ILLEGAL0		},
	[dv_iid_illegal1]	= {	2,	DV_INT_ILLEGAL1		},
	[dv_n_bcm2835_iid]	= {	99,	0					},	/* Avoids extra test in loop, without index-out-of-bounds */
};

dv_intlevel_t dv_bcm2835_irq_level[dv_n_bcm2835_iid];
dv_intlevel_t dv_currentlocklevel = -1;
dv_bcm2835_imask_t dv_bcm2835_irq_enabled;
dv_bcm2835_imask_t dv_bcm2835_levelmasks[DV_LOCKALL_LEVEL+1];	/* need levels from 0 to lock-all inclusive */

#ifndef DV_CFG_BCM2835_VECTOR_START
#define DV_CFG_BCM2835_VECTOR_START	0
#endif

void dv_bcm2835_interrupt_handler(void)
{
#if DV_DEBUG
	dv_printf("dv_bcm2835_interrupt_handler() - start\n");
#endif
	dv_bcm2835_imask_t pending;

#if DV_DEBUG
	print_interrupt_status(&dv_bcm2835_irq_enabled);
#endif

	/* Only consider interrupts that are enabled
	*/
	pending.mask[0] = dv_arm_bcm2835_interruptcontroller.irq_pending[0]	& dv_bcm2835_irq_enabled.mask[0];
	pending.mask[1] = dv_arm_bcm2835_interruptcontroller.irq_pending[1]	& dv_bcm2835_irq_enabled.mask[1];
	pending.mask[2] = dv_arm_bcm2835_interruptcontroller.basic_pending	& dv_bcm2835_irq_enabled.mask[2];

	/* Loop over all interrupt sources. Clear and handle the configured sources.
	*/
	dv_irqid_t irq = 0;
	for ( int reg = 0; reg < 3; reg++ )
	{
		dv_u32_t p = pending.mask[reg];
		while ( dv_bcm2835_irq_list[irq].idx == reg )
		{
			if ( (p & dv_bcm2835_irq_list[irq].mask) != 0 )
			{
				dv_softvector(irq);
			}
			irq++;
		}
	}
#if DV_DEBUG
	dv_printf("dv_bcm2835_interrupt_handler() - return\n");
#endif
}

/* dv_config_irq() - configure an irq
 *
 *		- set its level in the config array
 *		- set its bit in its mask in all levels where it is enabled
*/
void dv_config_irq(dv_irqid_t irq, dv_intlevel_t level, int unused_core)
{
	/* Constrain irq'ls level
	*/
	if ( level < 0 ) level = 0;
	if ( level > DV_MAX_INTLEVEL ) level = DV_MAX_INTLEVEL;

	/* Store level
	*/
	dv_bcm2835_irq_level[irq] = level;

	/* Set enable bit in each level where the irq is enabled
	*/
	for ( int i = 0; i <= level; i++ )
	{
		dv_bcm2835_levelmasks[i].mask[dv_bcm2835_irq_list[irq].idx] |= dv_bcm2835_irq_list[irq].mask;
#if DV_DEBUG
		dv_printf("dv_config_irq() - level %d, enabled = 0x%08x\n",
								i, dv_bcm2835_levelmasks[i].mask[dv_bcm2835_irq_list[irq].idx]);
#endif
	}
}
