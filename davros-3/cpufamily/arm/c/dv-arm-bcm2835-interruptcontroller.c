/*	dv-arm-bcm2835-interruptcontroller.c - BCM2835 interrupt controller functions
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
#include <kernel/h/dv-interrupt.h>
#include <cpufamily/arm/h/dv-arm-bcm2835-interruptcontroller.h>
#include <cpufamily/arm/h/dv-arm-dispatch.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-trace.h>
#include <kernel/h/dv-coverage.h>

/* dv_irq_handler() - interrupt handler (BCM2835 version)
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
 * At the end of an ISR, the physical interrupt source gets enabled again unless
 * the configured source has been disabled.
*/
void dv_irq_handler(dv_kernel_t *kvars)
{
	dv_u32_t pend[3];	/* 2 = basic, 0 = IRQ0, 1 = IRQ1. This order makes enable/disable easier. */
	int i;

	pend[0] = dv_arm_bcm2835_interruptcontroller.irq_pending[0];
	pend[1] = dv_arm_bcm2835_interruptcontroller.irq_pending[1];
	pend[2] = dv_arm_bcm2835_interruptcontroller.basic_pending;

	dv_kprintf("dv_irq_handler() called\n");
#if 0
	dv_kprintf(" -- kvars = 0x%08x, current_thread = 0x%08x, executable = 0x%08x\n",
						kvars, kvars->current_thread, kvars->current_thread->executable);
	dv_trace_dumpregs(kvars->current_thread->executable->name, kvars->current_thread->regs);
#endif
	dv_kprintf(" -- pend[0] = 0x%08x\n", pend[0]);
	dv_kprintf(" -- pend[1] = 0x%08x\n", pend[1]);
	dv_kprintf(" -- pend[2] = 0x%08x\n", pend[2]);

	/* Disable all the interrupts that are pending */
	dv_arm_bcm2835_interruptcontroller.irq_disable[0] = pend[0];
	dv_arm_bcm2835_interruptcontroller.irq_disable[1] = pend[1];
	dv_arm_bcm2835_interruptcontroller.irq_disable[2] = pend[2];

#if 0
	/* Loop over all interrupt sources. Clear and handle the configured sources.
	*/
	for ( i = 0; i < dv_n_softvector; i++ )
	{
		idx =  dv_softvector[i].cpu.irqidx;
		mask = dv_softvector[i].cpu.irqmask;
		if ( pend[idx] & mask )
		{
			pend[idx] &= ~mask;
			(*dv_softvector[i].handler)(dv_softvector[i].parameter);
		}
	}

	/* Report unknown interrupts and disable them.
	*/
	if ( (pend[0] | pend[1] | pend[2]) != 0)
	{
	}
#endif

	dv_resume_thread(kvars, kvars->current_thread);
}

/* man-page-generation - to be defined
*/
