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
 *
 *
 *	This file contains the main program of the boot monitor and loader.
 *
 *  Commands (not case sensitive):
 *		Sn....	- Type n S-Record 
 *		Ba		- display value of byte at location a
 *		Ha		- display value of 16-bit word at location a
 *		Wa		- display value of 32-bit word at location a
 *		Ba=v	- set byte at location a to v
 *		Ha=v	- set 16-bit word at location a to v
 *		Wa=v	- set 32-bit word at location a to v
 *		Da,l,s	- dump l words memory starting at a. Word size is s.
 *		Ma,s	- modify memory starting at a. Word size is s.
 *		Ga		- call subroutine at address a
 *		Q		- quit
 *
 *  Requires architecture-dependent functions or macros:
 *
 *		char readchar(void) - returns next character from input
 *			(serial port or whatever). Waits until available.
 *
 *		void writechar(char c) - writes character c to output (serial port
 *			or whatever). Waits until space available in output buffer.
 *
 *		uint8_t peek8(memaddr_t a) - returns the byte at address a.
 *		uint16_t peek16(memaddr_t a) - returns the 16-bit word at address a.
 *		uint32_t peek32(memaddr_t a) - returns the 32-bit word at address a.
 *
 *		void poke8(memaddr_t a, uint8_t v) - set the byte of memory at address a to v.
 *		void poke16(memaddr_t a, uint16_t v) - set the 16-bit word of memory at address a to v.
 *		void poke32(memaddr_t a, uint32_t v) - set the 32-bit word of memory at address a to v.
 *
 *		void go(memaddr_t a) - calls subroutine at a
 *
 *		ADDRSIZE - size of an address, in bits.
 *
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
    dv_kprintf("Davros monitor version 0.1!\n");
    dv_kprintf("Startup parameters:\n");
    dv_kprintf("  x0 = 0x%08x%08x\n", (dv_u32_t)(x0>>32), (dv_u32_t)(x0&0xffffffff));
    dv_kprintf("  x1 = 0x%08x%08x\n", (dv_u32_t)(x1>>32), (dv_u32_t)(x1&0xffffffff));
    dv_kprintf("  x2 = 0x%08x%08x\n", (dv_u32_t)(x2>>32), (dv_u32_t)(x2&0xffffffff));
    dv_kprintf("  x3 = 0x%08x%08x\n", (dv_u32_t)(x3>>32), (dv_u32_t)(x3&0xffffffff));

	monitor("mon > ");
}
