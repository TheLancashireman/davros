/*	dv-inithardware.c - ARM hardware initialisation source file for davros
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
#include DV_H_START
#include DV_H_MMU
#include DV_H_INTERRUPTCONTROLLER
#include DV_H_SYSTEMTIMER

/* dv_init_hardware() - initialise the processor etc.
*/
void dv_init_hardware(dv_kernel_t *kvars)
{
	dv_kprintf("dv_init_hardware()\n");

	dv_init_mmu(kvars);

	dv_init_interrupt_controller();
}

/* dv_init_peripherals() - initisalise the peripherals used by davros.
 *
 * Modifying vectors for reserved and banked interrupts is done here too.
*/
void dv_init_peripherals(dv_kernel_t *kvars)
{
	dv_kprintf("dv_init_peripherals()\n");

	dv_init_system_timer(kvars);
}
