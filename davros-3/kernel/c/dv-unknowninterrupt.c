/*	dv-unknowninterrupt.c - unknown interrupt handler for davros
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-interrupt.h>
#include <kernel/h/dv-error.h>

/* dv_unknown_interrupt() - handler for unknown interrupts
 *
 * This is a function to handle unknown interrupts.
 * Unknown interrupts are those that should never occur because there is no hardware to trigger them
 * or they are documented as "reserved" in the manuals.
*/
void dv_unknown_interrupt(dv_kernel_t *unused_kvars, dv_address_t param)
{
	dv_kprintf("dv_unknown_interrupt() - 0x%08x\n", (unsigned)param);
}
