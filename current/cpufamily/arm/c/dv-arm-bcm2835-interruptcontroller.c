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
#include <kernel/h/dv-coverage.h>

/* dv_irq_handler() - interrupt handler (BCM2835 version)
 *
 * The BCM2835 interrupt controller is a very simple unit. There is no vectoring, and
 * there are no priority levels.
 *
 * There are 3 sets of 3 registers for control and status related to the IRQ signal.
 * Each set of 3 has a base IRQ register and two IRQ registers. Annoyingly, the
 * "pending" set has the registers in a different order from the other two sets. :-(
*/
void dv_irq_handler(dv_kernel_t *kvars)
{
}

/* man-page-generation - to be defined
*/
