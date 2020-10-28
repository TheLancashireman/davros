/*	dv-qemu-uart.c - QEMU uart functions
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

typedef struct arm_uart_s arm_uart_t;

struct arm_uart_s
{
	volatile unsigned data;
	volatile unsigned status;
	unsigned reserved1[4];
	volatile unsigned flags;
	unsigned reserved2;
	volatile unsigned irda_lpcount;
	volatile unsigned i_brd;
	volatile unsigned f_brd;
	volatile unsigned lcr_h;
	volatile unsigned control;
	volatile unsigned ifls;
	volatile unsigned imsc;
	volatile unsigned ris;
	volatile unsigned mis;
	volatile unsigned icr;
	volatile unsigned dmacr;
};

#if 0
/* -machine integratorcp
*/
#define arm_uart0		(*(arm_uart_t *)0x16000000)
#define arm_uart1		(*(arm_uart_t *)0x17000000)
#else

/* -machine realview-pbx-a9
*/
#define arm_uart0		(*(arm_uart_t *)0x10009000)
#define arm_uart1		(*(arm_uart_t *)0x1000a000)
#define arm_uart2		(*(arm_uart_t *)0x1000b000)
#define arm_uart3		(*(arm_uart_t *)0x1000c000)
#endif

void dv_uart0_init(void)
{
	arm_uart0.control = 0x00;
	arm_uart0.i_brd = 0x30;
	arm_uart0.f_brd = 0x00;
	arm_uart0.lcr_h = 0x60;
	arm_uart0.control = 0x31;
}

void dv_uart0_putc(char c)
{
	/* Do we need to wait in qemu?
	*/
	arm_uart0.data = c;
}
