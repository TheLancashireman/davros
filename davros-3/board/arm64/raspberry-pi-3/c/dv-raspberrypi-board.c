/*	dv-raspberrypi3-board.c - Raspberry Pi3 board start for davros
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
#include <kernel/h/dv-kernel.h>
#include <dv-arm-bcm2835-uart.h>
#include <dv-arm-bcm2836.h>
#include <cpufamily/arm64/h/dv-arm64-core.h>
#include <kernel/h/dv-stdio.h>
#include <dv-string.h>

/* These are not really C data objects, just linker script labels.
*/
extern dv_u32_t dv_start_bss_c0, dv_end_bss_c0;

/* dv_board_start() - the board start function
 *
 * Initialises data/bss, initialises the uart, then calls dv_start()
 *
 * The 64-bit version of dv_reset passes 4 parameters to dv_board_start:
 *
 *	x0		- the intial value of the stack pointer
 *	x1..3	- the release addresses for cores 1..3 respectively.
 *
 *  To start a core, write the 32-bit address of a startup function into the release address.
*/
void dv_board_start(dv_u64_t x0, dv_u64_t x1, dv_u64_t x2, dv_u64_t x3)
{
	dv_u32_t el;

	/* data sections are already initialised by the loader.
	 * The bss sections are cleared here.
	*/
	dv_memset32(&dv_start_bss_c0, 0,
		((dv_address_t)&dv_end_bss_c0 - (dv_address_t)&dv_start_bss_c0 + sizeof(dv_u32_t) - 1) / sizeof(dv_u32_t));

	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	dv_kprintf("\n\n\n");
	dv_kprintf("********************************************************************************\n");
	dv_kprintf("Davros starting on Raspberry Pi\n");
	dv_kprintf("********************************************************************************\n");

	dv_init_core();

	el = dv_get_el();
	dv_kprintf("Current EL = %d\n", el);
	dv_kprintf("Dropping to EL2\n");
	dv_switch_el3el2();
	dv_kprintf("Dropped to EL2\n");

	el = dv_get_el();
	dv_kprintf("Current EL = %d\n", el);
	dv_kprintf("Dropping to EL1\n");
	dv_switch_el2el1();
	dv_kprintf("Dropped to EL1\n");

	el = dv_get_el();
	dv_kprintf("Current EL = %d\n", el);

	dv_start(0);
}

/* dv_catch_thread_irq() - handles IRQ interrupt requests from a thread
 *
 * The handler function must process all the known interrupts and then call the dispatcher.
 * Interrupt processing is performed by dv_bcm2836_interrupt_handler(), which in turn might call
 * the dv_bcm2835_interrupt_handler() function
*/
void dv_catch_thread_irq(dv_kernel_t *kvars)
{
	dv_bcm2836_interrupt_handler(kvars);
	dv_dispatch(kvars);
}
