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
#include <davroska-api.h>
#include <davroska.h>
#include <dv-arm-bcm2835-interruptcontroller.h>

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
dv_intlevel_t dv_currentlocklevel;
dv_bcm2835_imask_t dv_bcm2835_irq_enabled;
dv_bcm2835_imask_t dv_bcm2835_levelmasks[9];

#ifndef DV_CFG_BCM2835_VECTOR_START
#define DV_CFG_BCM2835_VECTOR_START	0
#endif

void dv_bcm2835_interrupt_handler(void)
{
	dv_bcm2835_imask_t pending;

	/* Only consider interrupts that are enabled
	*/
	pending.mask[0] = dv_arm_bcm2835_interruptcontroller.irq_pending[0]	& dv_bcm2835_irq_enabled.mask[0];
	pending.mask[1] = dv_arm_bcm2835_interruptcontroller.irq_pending[1]	& dv_bcm2835_irq_enabled.mask[1];
	pending.mask[2] = dv_arm_bcm2835_interruptcontroller.basic_pending	& dv_bcm2835_irq_enabled.mask[2];

	/* Loop over all interrupt sources. Clear and handle the configured sources.
	*/
	dv_irqid_t irq = 0;
	int reg = 0;
	for ( int i = 0; i < 3; i++ )
	{
		dv_u32_t p = pending.mask[i];
		while ( dv_bcm2835_irq_list[irq].idx == reg )
		{
			if ( (p & dv_bcm2835_irq_list[irq].mask) != 0 )
			{
				dv_softvector(i);
			}
			irq++;
		}
		reg = dv_bcm2835_irq_list[irq].idx;
	}
}