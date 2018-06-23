/*	loadhigh.c - a stub program to boot another program high in memory.
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
#include <devices/h/dv-arm-bcm2835-aux.h>
#include <devices/h/dv-arm-bcm2835-uart.h>
#include <kernel/h/dv-stdio.h>

extern const char bin_name[];
extern const unsigned char bin_array[];
extern const unsigned bin_length;
extern const unsigned bin_loadaddr;

typedef void (*fptr)(void);

/* loadhigh contains a copy of the binary of a different program that was
 * linked at a high address in RAM.
 *
 * loadhigh moves the program to its rightful place and then starts it.
 *
 * Assumptions:
 *	1. The program also contains a copy of dv-reset that catches all cores and puts three of them
 *	   to sleep until it needs them.
 *	2. The load address of the program is also the entry point.
*/

static inline void release_core(int c, dv_u32_t rel_addr, dv_u32_t entry)
{
	dv_kprintf("Release core %d at 0x%08x, rel_addr = 0x%08x\n", c, entry, rel_addr);
	*(dv_u32_t *)(dv_u64_t)rel_addr = entry;
}

static inline void wait_return(void)
{
	int c;
	for (;;)
	{
		c = dv_consoledriver.getc();
		if ( c == '\r' || c == '\n' )
			return;
	}
}

void dv_board_start(unsigned long x0, unsigned long x1, unsigned long x2, unsigned long x3)
{
	/* Enable the UART, then initialise it.
	*/
	dv_arm_bcm2835_enable(DV_AUX_uart);
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	/* Friendly greeting.
	*/
	dv_kprintf("Loadhigh version 0.1!\n");
	dv_kprintf("Parameters:\n");
    dv_kprintf("  x0 = 0x%08x%08x\n", (dv_u32_t)(x0>>32), (dv_u32_t)(x0&0xffffffff));
    dv_kprintf("  x1 = 0x%08x%08x\n", (dv_u32_t)(x1>>32), (dv_u32_t)(x1&0xffffffff));
    dv_kprintf("  x2 = 0x%08x%08x\n", (dv_u32_t)(x2>>32), (dv_u32_t)(x2&0xffffffff));
    dv_kprintf("  x3 = 0x%08x%08x\n", (dv_u32_t)(x3>>32), (dv_u32_t)(x3&0xffffffff));

	dv_kprintf("Testing the %%s format: %s\n", "it works!");

	/* Copy the data.
	*/
	dv_kprintf("Copying %s to 0x%08x\n", bin_name, bin_loadaddr);
	unsigned char *d = (unsigned char *)(dv_u64_t)bin_loadaddr;
	for ( unsigned i = 0; i < bin_length; i++ )
	{
		d[i] = bin_array[i];
	}
	dv_kprintf("Copied %u bytes to 0x%08x\n", bin_length, bin_loadaddr);

	dv_kprintf("Press RETURN to start each core in turn\n");

	/* Start the other cores.
	*/
	wait_return();
	release_core(1, x1, bin_loadaddr);
	wait_return();
	release_core(2, x2, bin_loadaddr);
	wait_return();
	release_core(3, x3, bin_loadaddr);
	wait_return();
	fptr x = (fptr)(dv_u64_t)bin_loadaddr;
	x();
	dv_kprintf("Oops! Loaded program returned!\n");
	for (;;) {}
}
