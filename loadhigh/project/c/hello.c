/*	hello.c - a test program for loadhigh
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

#define ARM64_MRS(regname) \
({	dv_u64_t MRSresult;                             \
	__asm__ volatile ("mrs %[result], " #regname    \
	: [result] "=r" (MRSresult)                     \
	: /* no inputs */                               \
	: /* nothing clobbered */);                     \
	MRSresult;                                      \
})

void hello(void)
{
	dv_u64_t mpidr = ARM64_MRS(MPIDR_EL1);
	int c = (int)(mpidr & 0x03);

	/* Set up console driver. UART is already initialized.
	*/
	dv_arm_bcm2835_uart_console();
	/* Friendly greeting.
	*/
	dv_kprintf("Hello from core %d\n", c);

#if 0
	for (;;)
	{
		__asm volatile ("wfe");
	}
#endif
}
