/*	dv-raspberrypi-board.c - Raspberry Pi board start for davros
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
#include <kernel/h/dv-kernel.h>
#include <devices//h/dv-arm-bcm2835-uart.h>
#include <kernel/h/dv-stdio.h>
#include <lib/h/dv-string.h>

/* These are not really C data objects, just linker script labels.
*/
extern dv_u32_t dv_start_bss_c0, dv_end_bss_c0;

void dv_board_start(int core_index)
{
	/* data sections are already initialised by the loader.
	 * The bss sections are cleared here.
	*/
	dv_memset32(&dv_start_bss_c0, 0,
		((dv_address_t)&dv_end_bss_c0 - (dv_address_t)&dv_start_bss_c0 + sizeof(dv_u32_t) - 1) / sizeof(dv_u32_t));
	
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	dv_kprintf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	dv_kprintf("********************************************************************************\n");
	dv_kprintf("Davros starting on Raspberry Pi\n");
	dv_kprintf("********************************************************************************\n");

	dv_start(core_index);
}
