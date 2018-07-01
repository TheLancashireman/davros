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
#include <devices/h/dv-arm-bcm2835-armtimer.h>
#include <kernel/h/dv-stdio.h>

/* Cores wait for an address to appear at the top of their stack.
*/
#define CORE1_SP	0x6000
#define CORE2_SP	0x4000
#define CORE3_SP	0x2000

extern void	dv_reset(void);

static inline void release_core(int c, dv_u32_t rel_addr, dv_u32_t entry)
{
#if 0
	rel_addr += (dv_u32_t)(dv_u64_t)&dv_reset;
	rel_addr -= 4;
#endif
	dv_kprintf("Release core %d at 0x%08x, rel_addr = 0x%08x\n", c, entry, rel_addr);
	*(dv_u32_t *)(dv_u64_t)rel_addr = entry;
}

static inline dv_u32_t timer_tick(void)
{
	return dv_arm_bcm2835_armtimer_read_frc();
}

static inline void timer_init(void)
{
	/* Need 1 MHz clock, so divier 250 MHz sysclock by 250.
	*/
	dv_arm_bcm2835_armtimer_set_frc_prescale(250);
	dv_arm_bcm2835_armtimer_enable_frc();
}

#define COUNTER_MASK	0x000F0000

void loop(dv_u32_t core_index, dv_u32_t match)
{
    while(1)
    {
        while(1)
        {
            if ( (timer_tick() & COUNTER_MASK) == match)
            {
				if ( core_index == 0 )
				{
					dv_consoledriver.putc('\r');
					dv_consoledriver.putc('\n');
				}
				dv_consoledriver.putc('0' + core_index);
                break;
            }
        }
        while(1)
        {
            if ( (timer_tick() & COUNTER_MASK) != match )
            {
                break;
            }
        }
    }
}

//-------------------------------------------------------------------
void enter_one(void)
{
	loop(1, 0x00010000);
}
//-------------------------------------------------------------------
void enter_two(void)
{
	loop(2, 0x00020000);
}
//-------------------------------------------------------------------
void enter_three(void)
{
	loop(3, 0x00030000);
}
//-------------------------------------------------------------------
void dv_board_start(unsigned long x0, unsigned long x1, unsigned long x2, unsigned long x3)
{
	/* Enable the UART, then initialise it.
	*/
	dv_arm_bcm2835_enable(DV_AUX_uart);
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	/* Friendly greeting.
	*/
	dv_kprintf("Hello, world!\n");
	dv_kprintf("version %d\n", 6);

    dv_kprintf("0x%08x\n", 0x12345678);
	dv_kprintf("Testing the %%s format: %s\n", "it works!");

    dv_kprintf("x0 = 0x%08x%08x\n", (dv_u32_t)(x0>>32), (dv_u32_t)(x0&0xffffffff));
    dv_kprintf("x1 = 0x%08x%08x\n", (dv_u32_t)(x1>>32), (dv_u32_t)(x1&0xffffffff));
    dv_kprintf("x2 = 0x%08x%08x\n", (dv_u32_t)(x2>>32), (dv_u32_t)(x2&0xffffffff));
    dv_kprintf("x3 = 0x%08x%08x\n", (dv_u32_t)(x3>>32), (dv_u32_t)(x3&0xffffffff));

    dv_kprintf("0x%08x\n", 0x12345678);
    timer_init();

	/* Start the other cores.
	*/
	release_core(1, x1, (dv_u32_t)(dv_u64_t)enter_one);
	release_core(2, x2, (dv_u32_t)(dv_u64_t)enter_two);
	release_core(3, x3, (dv_u32_t)(dv_u64_t)enter_three);

	/* Go to the loop myself.
	*/
	loop(0, 0x00000000);
}
