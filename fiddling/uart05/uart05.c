#include <devices/h/dv-arm-bcm2835-aux.h>
#include <devices/h/dv-arm-bcm2835-uart.h>
#include <devices/h/dv-arm-bcm2835-armtimer.h>
#include <kernel/h/dv-stdio.h>

#define go_el3_el2	dv_switch_el3el2
#define go_el2_el1	dv_switch_el2el1
#define notmain		dv_board_start

#define ARM64_MRS(regname) \
({	dv_u64_t MRSresult;								\
	__asm__ volatile ("mrs %[result], " #regname	\
	: [result] "=r" (MRSresult)						\
	: /* no inputs */								\
	: /* nothing clobbered */);						\
	MRSresult;										\
})

#define ARM64_MSR(regname, value) \
({	__asm__ volatile ("msr " #regname ", %[val]"	\
	: /* no outputs */								\
	: [val] "r" (value)								\
	: /* nothing clobbered */);						\
})

#define GetMyCoreId() \
({	dv_u64_t GMCI_rv;				\
	GMCI_rv = ARM64_MRS(MPIDR_EL1);	\
	GMCI_rv;						\
})

/* Cores wait for an address to appear at the top of their stack.
*/
#define CORE1_SP	0x6000
#define CORE2_SP	0x4000
#define CORE3_SP	0x2000

void go_el3_el1(void);
void go_el3_el2(void);
void go_el2_el1(void);

static inline void release_core(int c, dv_u32_t rel_addr, dv_u32_t entry)
{
	dv_kprintf("Release core %d at 0x%08x\n", c, entry);
	*(dv_u32_t *)(dv_u64_t)rel_addr = entry;
}

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
void enter_one ( void )
{
	loop(1, 0x00010000);
}
//-------------------------------------------------------------------
void enter_two ( void )
{
	loop(2, 0x00020000);
}
//-------------------------------------------------------------------
void enter_three ( void )
{
	loop(3, 0x00030000);
}
//-------------------------------------------------------------------
void init_core(void)
{
	dv_u64_t mpidr = GetMyCoreId();
	dv_kprintf("MPIDR_EL1 = 0x%08x%08x\n", (dv_u32_t)(mpidr>>32), (dv_u32_t)mpidr);

	dv_u64_t current_el = ARM64_MRS(CurrentEL);
	int el = (current_el>>2) & 3;
	dv_kprintf("CurrentEL = 0x%08x (el%d)\n", (dv_u32_t)current_el, el);

	dv_kprintf("msr     DAIFSet, 0xf\n");
	__asm__ volatile ("msr DAIFSet, 0xf");

	if ( el >= 3 )
	{
		dv_kprintf("ARM64_MSR(ELR_EL3, 0);\n");
		ARM64_MSR(ELR_EL3, 0);
		dv_kprintf("ARM64_MSR(VBAR_EL3, 0);\n");
		ARM64_MSR(VBAR_EL3, 0);
#if 1
		dv_kprintf("ARM64_MSR(SCR_EL3, 0xc81);\n");		/* NS = 1 */
		ARM64_MSR(SCR_EL3, 0xc81);
#else
		dv_kprintf("ARM64_MSR(SCR_EL3, 0xc80);\n");		/* NS = 0 means no EL2 */
		ARM64_MSR(SCR_EL3, 0xc80);
#endif
		dv_kprintf("ARM64_MSR(SCTLR_EL3, 0);\n");
		ARM64_MSR(SCTLR_EL3, 0);
		dv_kprintf("ARM64_MSR(CPTR_EL3, 0);\n");
		ARM64_MSR(CPTR_EL3, 0);
		dv_kprintf("ARM64_MSR(MDCR_EL3, 0);\n");
		ARM64_MSR(MDCR_EL3, 0);
	}

	if ( el >= 2 )
	{
		dv_kprintf("ARM64_MSR(ELR_EL2, 0);\n");
		ARM64_MSR(ELR_EL2, 0);
		dv_kprintf("ARM64_MSR(VBAR_EL2, 0);\n");
		ARM64_MSR(VBAR_EL2, 0);
		dv_kprintf("ARM64_MSR(HCR_EL2, 0x80000000);\n");
		ARM64_MSR(HCR_EL2, 0x80000000);
		dv_kprintf("ARM64_MSR(SCTLR_EL2, 0);\n");
		ARM64_MSR(SCTLR_EL2, 0);
		dv_kprintf("ARM64_MSR(VTTBR_EL2, 0);\n");
		ARM64_MSR(VTTBR_EL2, 0);
		dv_kprintf("ARM64_MSR(CPTR_EL2, 0);\n");
		ARM64_MSR(CPTR_EL2, 0);
		dv_kprintf("ARM64_MSR(MDCR_EL2, 0);\n");
		ARM64_MSR(MDCR_EL2, 0);
		dv_kprintf("ARM64_MSR(CNTVOFF_EL2, 0);\n");
		ARM64_MSR(CNTVOFF_EL2, 0);
	}

	if ( el >= 1 )
	{
		dv_kprintf("ARM64_MSR(ELR_EL1, 0);\n");
		ARM64_MSR(ELR_EL1, 0);
		dv_kprintf("ARM64_MSR(SPSR_EL1, 0);\n");
		ARM64_MSR(SPSR_EL1, 0);
		dv_kprintf("ARM64_MSR(SCTLR_EL1, 0);\n");
		ARM64_MSR(SCTLR_EL1, 0);

		dv_kprintf("tlbi ALLE1\n");
		__asm__ volatile("tlbi ALLE1");

		dv_u64_t cpacr = ARM64_MRS(CPACR_EL1)|0x300000;
		dv_kprintf("ARM64_MSR(CPACR_EL1, 0x%08x);\n", (dv_u32_t)cpacr);
		ARM64_MSR(CPACR_EL1, cpacr|0x300000);		/* FPEN = 11 */

		dv_kprintf("ARM64_MSR(S3_1_C15_C2_1, 0x40);  (S3_1_C15_C2_1 is CPUECTLR_EL1)\n");
		ARM64_MSR(S3_1_C15_C2_1, 0x40);
	}
	else
	{
		dv_kprintf("PANIC! Started at EL0\n");
	}

	if ( el == 3 )
	{
		dv_kprintf("CurrentEL = 0x%08x (el%d)\n", (dv_u32_t)current_el, el);
		dv_kprintf("go_el3_el2()\n");
		go_el3_el2();

		current_el = ARM64_MRS(CurrentEL);
		el = (current_el>>2) & 3;
	}

	if ( el == 2 )
	{
		dv_kprintf("CurrentEL = 0x%08x (el%d)\n", (dv_u32_t)current_el, el);
		dv_kprintf("go_el2_el1()\n");
		go_el2_el1();

		current_el = ARM64_MRS(CurrentEL);
		el = (current_el>>2) & 3;
	}

	dv_kprintf("CurrentEL = 0x%08x (el%d)\n", (dv_u32_t)current_el, el);
	dv_kprintf("Initialization complete!\n");
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
	dv_kprintf("uart05 version %d\n", 1);

	init_core();

	dv_kprintf("timer_init();\n");	
    timer_init();

	/* Start the other cores.
	*/
	release_core(1, CORE1_SP-16, (dv_u32_t)(dv_u64_t)enter_one);
	release_core(2, CORE2_SP-16, (dv_u32_t)(dv_u64_t)enter_two);
	release_core(3, CORE3_SP-16, (dv_u32_t)(dv_u64_t)enter_three);

	/* Go to the loop myself.
	*/
	loop(0, 0x00000000);

	/*	Not reached.
	*/
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2016 David Welch dwelch@dwelch.com
// Modified by David Haworth 2018
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
