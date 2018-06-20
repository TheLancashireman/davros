#include <devices/h/dv-arm-bcm2835-aux.h>
#include <devices/h/dv-arm-bcm2835-uart.h>
#include <devices/h/dv-arm-bcm2835-armtimer.h>
#include <kernel/h/dv-stdio.h>

/* Cores wait for an address to appear at the top of their stack.
*/
#define CORE1_SP	0x6000
#define CORE2_SP	0x4000
#define CORE3_SP	0x2000

static inline void release_core(int c, dv_u32_t rel_addr, dv_u32_t entry)
{
	dv_kprintf("Release core %d at 0x%08x\n", c, entry);
	*(dv_u32_t *)(dv_u64_t)rel_addr = entry;
}

#if 0
extern void PUT32 ( unsigned int, unsigned int );
#endif

extern unsigned int GETPC(void);	/* Implemented in assembler; we don't need no optimisation! */

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

//-------------------------------------------------------------------
void enter_one ( void )
{

    while(1)
    {
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x00010000)
            {
				dv_consoledriver.putc('1');
                break;
            }
        }
        while(1)
        {
            if((timer_tick()&0x000F0000)!=0x00010000)
            {
                break;
            }
        }
    }
}
//-------------------------------------------------------------------
void enter_two ( void )
{

    while(1)
    {
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x00020000)
            {
				dv_consoledriver.putc('2');
                break;
            }
        }
        while(1)
        {
            if((timer_tick()&0x000F0000)!=0x00020000)
            {
                break;
            }
        }
    }
}
//-------------------------------------------------------------------
void enter_three ( void )
{

    while(1)
    {
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x00030000)
            {
				dv_consoledriver.putc('3');
                break;
            }
        }
        while(1)
        {
            if((timer_tick()&0x000F0000)!=0x00030000)
            {
                break;
            }
        }
    }
}
//-------------------------------------------------------------------
int notmain ( void )
{
/* Enable the UART, then initialise it.
*/
	dv_arm_bcm2835_enable(DV_AUX_uart);
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

/* Friendly greeting.
*/
	dv_kprintf("Hello, world!\n");
	dv_kprintf("version %d\n", 3);

    dv_kprintf("0x%08x\n", 0x12345678);
    dv_kprintf("0x%08x\n", GETPC());
    timer_init();

#if 0
    //gave up trying to get the compiler warning to go away
    {
        unsigned long la;
    	unsigned int ra;
        la=(unsigned long)enter_one;
        ra=la&0xFFFFFFFF;
        dv_kprintf("0x%08x\n", ra);
        PUT32(0x6000,ra);
        la=(unsigned long)enter_two;
        ra=la&0xFFFFFFFF;
        dv_kprintf("0x%08x\n", ra);
        PUT32(0x4000,ra);
        la=(unsigned long)enter_three;
        ra=la&0xFFFFFFFF;
        dv_kprintf("0x%08x\n", ra);
        PUT32(0x2000,ra);
    }
#endif

/* Start the other cores.
*/
	release_core(1, CORE1_SP, (dv_u32_t)(dv_u64_t)enter_one);
	release_core(2, CORE2_SP, (dv_u32_t)(dv_u64_t)enter_two);
	release_core(3, CORE3_SP, (dv_u32_t)(dv_u64_t)enter_three);

    while(1)
    {
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x00000000)
            {
				dv_consoledriver.putc('0');
                break;
            }
        }
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x000E0000)
            {
				dv_consoledriver.putc('\r');
                break;
            }
        }
        while(1)
        {
            if((timer_tick()&0x000F0000)==0x000F0000)
            {
				dv_consoledriver.putc('\n');
                break;
            }
        }
    }
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2016 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
