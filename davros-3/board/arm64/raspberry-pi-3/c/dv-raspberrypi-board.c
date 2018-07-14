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
#include <devices//h/dv-arm-bcm2835-uart.h>
#include <cpufamily/arm64/h/dv-arm64-core.h>
#include <kernel/h/dv-stdio.h>
#include <lib/h/dv-string.h>

/* These are not really C data objects, just linker script labels.
*/
extern dv_u32_t dv_start_bss_c0, dv_end_bss_c0;

/* The 64-bit version of dv_reset passes 4 parameters to dv_board_start:
 *
 *	x0		- the intial value of the stack pointer
 *	x1..3	- the release addresses for cores 1..3 respectively.
 *
 *  To start a core, write the 32-bit address of a startup function into the release address.
*/
void dv_board_start(dv_u64_t x0, dv_u64_t x1, dv_u64_t x2, dv_u64_t x3)
{
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

	dv_u64_t el;
	el = dv_arm64_mrs(CurrentEL);
	dv_kprintf("Current EL = 0x%08x%08x\n", (dv_u32_t)(el >> 32), (dv_u32_t)(el & 0xffffffff));
	dv_kprintf("Dropping to EL2\n");
	dv_switch_el1(0x00000009);	/* DAIF = 0, M[4:0] = 9 (EL2h must match SCR_EL3.RW) */
	dv_kprintf("Dropped to EL2\n");
	el = dv_arm64_mrs(CurrentEL);
	dv_kprintf("Current EL = 0x%08x%08x\n", (dv_u32_t)(el >> 32), (dv_u32_t)(el & 0xffffffff));
	dv_kprintf("Dropping to EL1\n");
	dv_switch_el1(0x00000005);	/* DAIF = 0, M[4:0] = 5 (EL1h must match HCR_EL2.RW) */
	dv_kprintf("Dropped to EL1\n");
	el = dv_arm64_mrs(CurrentEL);
	dv_kprintf("Current EL = 0x%08x%08x\n", (dv_u32_t)(el >> 32), (dv_u32_t)(el & 0xffffffff));


	dv_start(0);
}
