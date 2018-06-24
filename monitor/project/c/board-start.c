/*	board-start.c - startup function for monitor and bootloader
 *
 *	Copyright 2001 David Haworth
 *
 *	This file is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2, or (at your option)
 *	any later version.
 *
 *	It is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; see the file COPYING.  If not, write to
 *	the Free Software Foundation, 59 Temple Place - Suite 330,
 *	Boston, MA 02111-1307, USA.
*/
#include <project/h/monitor.h>
#include <devices/h/dv-arm-bcm2835-aux.h>
#include <devices/h/dv-arm-bcm2835-uart.h>
#include <kernel/h/dv-stdio.h>

void dv_board_start(unsigned long x0, unsigned long x1, unsigned long x2, unsigned long x3)
{
    /* Enable the UART, then initialise it.
    */
    dv_arm_bcm2835_enable(DV_AUX_uart);
    dv_arm_bcm2835_uart_init(115200, 8, 0);
    dv_arm_bcm2835_uart_console();

    /* Friendly greeting.
    */
    dv_kprintf("Davros monitor version 0.2\n");
    dv_kprintf("Startup parameters:\n");
    dv_kprintf("  x0 = 0x%08x%08x\n", (dv_u32_t)(x0>>32), (dv_u32_t)(x0&0xffffffff));
    dv_kprintf("  x1 = 0x%08x%08x\n", (dv_u32_t)(x1>>32), (dv_u32_t)(x1&0xffffffff));
    dv_kprintf("  x2 = 0x%08x%08x\n", (dv_u32_t)(x2>>32), (dv_u32_t)(x2&0xffffffff));
    dv_kprintf("  x3 = 0x%08x%08x\n", (dv_u32_t)(x3>>32), (dv_u32_t)(x3&0xffffffff));

	monitor("mon > ");
}
