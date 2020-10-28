/*	dv-arm-bcm2835-interruptcontroller.c - BCM2835 interrupt controller functions
 *
 *	Copyright David Haworth
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
#include <dv-arm-bcm2835-interruptcontroller.h>
#if DV_DAVROSKA
#else
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-interrupt.h>
#include <cpufamily/arm/h/dv-arm-dispatch.h>
#endif

/* dv_bcm2835_interrupt_handler() - interrupt handler (davros-3 version)
 *
 * The BCM2835 interrupt controller is a very simple unit. There is no vectoring, and
 * there are no priority levels.
 *
 * The first step in this function is to read the pending registers into an array.
 * This gives us a way of configuring IRQs and handling them;
 * Each configured IRQ has an index and a mask. When the software accepts the
 * interrupt it clears the pending bit in the array and disables the interrupt.
 *
 * After processing all the known IRQs, if there are any left they are reported as unknown
 * and disabled.
 *
 * Interrupts are handled by calling the function from the corresponding
 * entry in the software vector table. The standard interrupt dispatcher cannot be used because
 * it is designed to handle a single interrupt, whereas we must handle every interrupt that's
 * pending. Before calling the handler the source is disabled. This means that if the handler
 * must re-enable the interrupt if it has been completely handled within the kernel. If the
 * handler spawns an "ISR" thread, the kernel must re-enable the interrupt source
 * when the ISR terminates.
 *
 * dv_bcm2835_irq_list[] is a table of all the interrupt sources that can be used.
 * The index in this list gives the vector number of the IRQ.
 * The list must be ordered so that the register numbers are clustered in order 0,1,2.
*/
const dv_bcm2835_irq_t dv_bcm2835_irq_list[dv_n_bcm2835_iid] =
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
};

static const dv_bcm2835_imask_t known_irq =
{	{
		[0] =	DV_INT_SYST_CM1 |
				DV_INT_SYST_CM3 |
				DV_INT_AUX,

		[1] =	DV_INT_I2CSPI_SLV |
				DV_INT_PWA0 |
				DV_INT_PWA1 |
				DV_INT_SMI |
				DV_INT_GPIO0 |
				DV_INT_GPIO1 |
				DV_INT_GPIO2 |
				DV_INT_GPIO3 |
				DV_INT_I2C |
				DV_INT_SPI |
				DV_INT_PCM |
				DV_INT_UART,

		[2] =	DV_INT_TIMER |
				DV_INT_MAILBOX |
				DV_INT_DOORBELL0 |
				DV_INT_DOORBELL1 |
				DV_INT_GPU0HALT |
				DV_INT_GPU1HALT |
				DV_INT_ILLEGAL0 |
				DV_INT_ILLEGAL1
	}
};

#ifndef DV_CFG_BCM2835_VECTOR_START
#define DV_CFG_BCM2835_VECTOR_START	0
#endif

void dv_bcm2835_interrupt_handler(dv_kernel_t *kvars)
{
	dv_bcm2835_imask_t pending;
	int i, idx, vecidx;
	dv_u32_t mask;

	/* Only consider the IRQs that are documented. There are others that appear to be used
 	 * by the GPU. At least one (basic_irq 0x100) seems to follow the CM1 IRQ :-?
	*/
	pending.mask[0] = dv_arm_bcm2835_interruptcontroller.irq_pending[0];
	pending.mask[1] = dv_arm_bcm2835_interruptcontroller.irq_pending[1];
	pending.mask[2] = dv_arm_bcm2835_interruptcontroller.basic_pending;

	DV_DBG(dv_kprintf("dv_bcm2835_interrupt_handler() called\n"));
	DV_DBG(dv_kprintf(" -- pend[0] = 0x%08x\n", pending.mask[0]));
	DV_DBG(dv_kprintf(" -- pend[1] = 0x%08x\n", pending.mask[1]));
	DV_DBG(dv_kprintf(" -- pend[2] = 0x%08x\n", pending.mask[2]));


	/* Disable all the interrupts that are pending */
	dv_arm_bcm2835_interruptcontroller.irq_disable[0] = pending.mask[0];
	dv_arm_bcm2835_interruptcontroller.irq_disable[1] = pending.mask[1];
	dv_arm_bcm2835_interruptcontroller.irq_disable[2] = pending.mask[2];

	/* Loop over all interrupt sources. Clear and handle the configured sources.
	*/
	for ( i = 0; i < dv_n_bcm2835_iid; i++ )
	{
		idx =  dv_bcm2835_irq_list[i].idx;
		mask = dv_bcm2835_irq_list[i].mask;
		if ( pending.mask[idx] & mask )
		{
			pending.mask[idx] &= ~mask;
			vecidx = DV_CFG_BCM2835_VECTOR_START+i;
			(*dv_softvector[vecidx].handler)(kvars, dv_softvector[vecidx].parameter);
		}
	}

	/* Report unknown interrupts.
	*/
	pending.mask[0] &= known_irq.mask[0];
	pending.mask[1] &= known_irq.mask[1];
	pending.mask[2] &= known_irq.mask[2];
	if ( (pending.mask[0] | pending.mask[1] | pending.mask[2]) != 0)
	{
		dv_kprintf("dv_bcm2835_interrupt_handler() - unknown interrupts: 0x%08x, 0x%08x, 0x%08x\n",
					pending.mask[0], pending.mask[1], pending.mask[2]);
	}
}
