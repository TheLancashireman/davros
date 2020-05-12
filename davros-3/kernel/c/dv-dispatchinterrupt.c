/*	dv-dispatchinterrupt.c - interrupt dispatcher for davros
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
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-interrupt.h>
#include DV_H_DISPATCH

DV_COVDEF(dv_dispatch_interrupt);

/* If there's only one core, the SPI and PPI vectors are done directly, so
 * the size of the software vector table is exactly the number of interrupt sources.
 * If there are 2 or more cores, the vectors 1-15 and 27-31 have to be demultiplexed.
 * That means 20 extra vectors per core. Interrupt 0 is the cross-core interrupt,
 * so it doesn't need to be demultiplexed.
*/
dv_softvector_t dv_softvector[DV_N_SOFTVECTOR];

/* dv_attach_irq() - attach a handler to an IRQ
*/
void dv_attach_irq(int iid, dv_inthandler_t f, dv_address_t p)
{
	dv_softvector[iid].handler = f;
	dv_softvector[iid].parameter = p;
}

/* dv_init_softvector() - initialise the software vector table.
 *
 * Multi-core - run this only on one core.
*/
void dv_init_softvector(void)
{
	int i;

	/* Set all the hardware vectors to "unconfigured".
	*/
	for ( i = 0; i < DV_N_IID; i++ )
		dv_attach_irq(i, dv_unconfigured_interrupt, i);

	/* Set any remaining vectors (used for demultiplexing "banked" interrupts, etc.)
	 * to "unknown".
	*/
	for ( ; i < DV_N_SOFTVECTOR; i++ )
		dv_attach_irq(i, dv_unknown_interrupt, i);
}

#if 0	/* This function isn't always needed. ToDo: proper selection */
/* dv_dispatch_interrupt() - dispatch an interrupt
 *
 * Select a function from a table using the supplied interrupt id, and call it.
 *
 * The selected function could:
 *	- report an unknown (reserved) interrupt
 *	- report an unconfigured interrupt
 *	- handle the interrupt internally in davros (e.g. crosscore, timer)
 *	- start an executable to handle the the interrupt
 *	- etc.
 *
 * Prerequisites:
 * 	- the iid has been forced into the range 0 to XXX by the caller.
 *
 * ToDo: XXX is larger than the largest HW id because of demultiplexing.
*/
void dv_dispatch_interrupt(dv_kernel_t *kvars, unsigned iid)
{
	DV_DBG(dv_kprintf("dv_dispatch_interrupt: called ion core %d with vector %d\n", kvars->core_index, iid));
	(*dv_softvector[iid].handler)(kvars, dv_softvector[iid].parameter);

	/* If the interrupt handler didn't dispatch then just return to the interrupted thread.
	*/
	dv_return_to_thread(kvars, kvars->current_thread);
}
#endif
