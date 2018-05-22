#include "uart.h"

#define xx_NO_RELEASE		0
#define xx_UBOOT_RELEASE	1
#define xx_RELEASE_F0		2
#define xx_RELEASE_E0_E8_F0	3
#define xx_RELEASE_MBOX3	4

//#define xx_RELEASE_METHOD	xx_RELEASE_MBOX3

#ifndef xx_RELEASE_METHOD
#define xx_RELEASE_METHOD	xx_NO_RELEASE
#endif

typedef unsigned u32;
typedef unsigned long u64;
void print_u64(u64 x);
void print_u32(u32 x);

#if xx_RELEASE_METHOD == xx_UBOOT_RELEASE
#define CPU_RELEASE_ADDR	0xffffff0		/* From uboot */
#endif

#if xx_RELEASE_METHOD == xx_RELEASE_F0
#define CPU_RELEASE_ADDR	0x000000f0		/* From linux arch/arm64/boot/dts/broadcom/bcm2837.dtsi */
#endif

#if xx_RELEASE_METHOD == xx_RELEASE_E0_E8_F0
#define CPU_RELEASE_ADDR_C1	0x000000e0		/* From https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=212930 */
#define CPU_RELEASE_ADDR_C2	0x000000e8
#define CPU_RELEASE_ADDR_C3	0x000000f0
#endif

#define ARM64_MRS(regname) \
({	u64 MRSresult;									\
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
({	u64 GMCI_rv;					\
	GMCI_rv = ARM64_MRS(MPIDR_EL1);	\
	GMCI_rv;						\
})

extern int _start0, _start1, _start2, _start3;
void go_el1(void *addr, u64 psr);

void *const coreStartAddr[4] =
{	&_start0,
	&_start1,
	&_start2,
	&_start3
};

/* See "Circle" project /data1/projects/OtherPeople/Circle
*/
#define ARM_LOCAL_BASE				0x40000000
#define ARM_LOCAL_MAILBOX3_SET0		(ARM_LOCAL_BASE + 0x08C)
#define ARM_LOCAL_MAILBOX3_CLR0		(ARM_LOCAL_BASE + 0x0CC)

volatile u64 this_cpuid = 0;

const char welcome[] = "Hello world!\n\n";

void play(void)
{
	uart_init();
	print_u64(0xfedcba9876543210);
	uart_write("\n");

	u64 my_cpuid = GetMyCoreId();

	uart_write("my_cpuid = ");
	print_u64(my_cpuid);
	uart_write("\n");

	uart_write("this_cpuid = ");
	print_u64(this_cpuid);
	uart_write("\n");

	u64 current_el = ARM64_MRS(CurrentEL);
	uart_write("current_el = ");
	print_u64(current_el);
	uart_write("\n");

	u64 rrr = ARM64_MRS(SCTLR_EL2);
	uart_write("SCTLR_EL2 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(SCTLR_EL1);
	uart_write("SCTLR_EL1 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(TCR_EL2);
	uart_write("TCR_EL2 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(TCR_EL1);
	uart_write("TCR_EL1 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(TTBR0_EL2);
	uart_write("TTBR0_EL2 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(TTBR0_EL1);
	uart_write("TTBR0_EL1 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(TTBR1_EL1);
	uart_write("TTBR1_EL1 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(VTTBR_EL2);
	uart_write("VTTBR_EL2 = ");
	print_u64(rrr);
	uart_write("\n");

	rrr = ARM64_MRS(VTCR_EL2);
	uart_write("VTCR_EL2 = ");
	print_u64(rrr);
	uart_write("\n");

#if 0
	/* Disabling the cache causes constant stream of U in the UART.
	 * Disabling MMU (with or without cache) causes trap.
	*/
	uart_write("Disabling cache in SCTRL_EL2\n");
	rrr = ARM64_MRS(SCTLR_EL2);
	uart_write("Old value = ");
	print_u64(rrr);
	rrr &= ~0x04;
	uart_write("   new value = ");
	print_u64(rrr);
	uart_write("\n");
	ARM64_MSR(SCTLR_EL2, rrr);
#endif

	if ( my_cpuid == 0x0000000080000000 )
	{
		uart_write(welcome);
		this_cpuid = my_cpuid;
	}

#if 0
	/* Not sure what happened. No more UART output.
	*/
	if ( current_el == 8 )
	{
		uart_write("Trying to drop to EL1\n");
		ARM64_MSR(SCTLR_EL1, 0);
		rrr = ARM64_MRS(HCR_EL2);
		uart_write("HCR_EL2 (old) = ");
		print_u64(rrr);
		uart_write("\n");
		rrr |= 0x80000000;		/* Set execution state to AARCH64 */
		uart_write("HCR_EL2 (new) = ");
		print_u64(rrr);
		uart_write("\n");
		ARM64_MSR(HCR_EL2, rrr);
		uart_write("go_el1()...\n");
		go_el1(&_start0, 0x00000005);	/* DAIF = 0, M[4:0] = 5 (EL1h must match HCR_EL2.RW */
		uart_write("Oops ... go_el1() returned :-(\n");
	}
#endif

#if 0
	uart_write("Contents of TT0\n");
	u64 *p = (u64*)ARM64_MRS(TTBR0_EL2);
	u32 i;
    u64 l = 0xffffffffffffffff;
	for ( i = 0; i < 8192; i++, p++ )
	{
		if ( *p != l )
		{
			print_u32(i);
			uart_write(" (");
			print_u64((u64)p);
			uart_write(") = ");
			print_u64(*p);
			uart_write("\n");
			l = *p;
		}
	}
#if 0
	uart_write("Contents of TT1\n");
	p = (u64*)ARM64_MRS(TTBR0_EL2);
	p = (u64*)((*p) & ~0xfff);
	l = 0xffffffffffffffff;
	for ( i = 0; i < 512; i++, p++ )
	{
		if ( *p != l )
		{
			print_u32(i);
			uart_write(" (");
			print_u64((u64)p);
			uart_write(") = ");
			print_u64(*p);
			uart_write("\n");
			l = *p;
		}
	}
	uart_write("Contents of TT2\n");
	p = (u64*)ARM64_MRS(TTBR0_EL2);
	p = (u64*)((*p) & ~0xfff);
	p = (u64*)((*p) & ~0xfff);
	l = 0xffffffffffffffff;
	for ( i = 0; i < 512; i++, p++ )
	{
		if ( *p != l )
		{
			print_u32(i);
			uart_write(" (");
			print_u64((u64)p);
			uart_write(") = ");
			print_u64(*p);
			uart_write("\n");
			l = *p;
		}
	}
#endif
#endif

	while ( this_cpuid != my_cpuid )
	{
		/* Spin here until released by preceding core */
		 uart_putc('0' + (my_cpuid & 0x03));
		volatile int i;

		for ( i=0; i < 100000; i++ )
		{
		}
	}

	if (my_cpuid == 0x0000000080000000 )
	{
		uart_write("Trying to start other cores\n");

#if xx_RELEASE_METHOD == xx_UBOOT_RELEASE || xx_RELEASE_METHOD == xx_RELEASE_F0

		uart_write("Setting CPU_RELEASE_ADDR to ");
		u64 release_addr = (u64)&_start0;
		print_u64(release_addr);
		uart_write("\n");
		*(u64 *)CPU_RELEASE_ADDR = release_addr;

		uart_write("Executing SEV\n");
		__asm__ __volatile__("dsb sy");
		__asm__ __volatile__("isb");
		__asm__ __volatile__("sev");

#elif xx_RELEASE_METHOD == xx_RELEASE_E0_E8_F0
		uart_write("Setting CPU_RELEASE_ADDR_C1 to ");
		u64 release_addr = (u64)&_start1;
		print_u64(release_addr);
		uart_write("\n");
		*(u64 *)CPU_RELEASE_ADDR_C1 = release_addr;

		uart_write("Executing SEV\n");
		__asm__ __volatile__("dsb sy");
		__asm__ __volatile__("isb");
		__asm__ __volatile__("sev");

#elif xx_RELEASE_METHOD == xx_RELEASE_MBOX3
		__asm__ __volatile__("dsb sy");
#if 0
		while ( (xx = ((u32 *)(ARM_LOCAL_MAILBOX3_CLR0 + 0x10))[0]) != 0 )
		{
			uart_putc('+');
		}
#endif
		((u32 *)(ARM_LOCAL_MAILBOX3_SET0 + 0x10))[0] =  (u32)&_start1;
		__asm__ __volatile__("dsb sy");
		__asm__ __volatile__("isb");
		__asm__ __volatile__("sev");
#if 0
		while ( (xx = ((u32 *)(ARM_LOCAL_MAILBOX3_CLR0 + 0x10))[0]) != 0 )
		{
			uart_putc('-');
		}
#endif
#else
	 uart_write("No release method!\n");
#endif
	}

	uart_write("Going endless ...\n");

	this_cpuid++;

	while (1)
	{
	}
}

char buf[128];		/* Watch out! core contention! */

void print_u64(u64 x)
{
	int i;
	char c;

	for ( i=0; i<16; i++ )
	{
		c = (char)(x & 0x0f);
		x >>= 4;
		if ( c >= 10 )
			c += 'a' - 10;
		else
			c += '0';
		buf[15-i] = c;
	}
	buf[16] = '\0';
	uart_write(buf);
}

void print_u32(u32 x)
{
	int i;
	char c;

	for ( i=0; i<8; i++ )
	{
		c = (char)(x & 0x0f);
		x >>= 4;
		if ( c >= 10 )
			c += 'a' - 10;
		else
			c += '0';
		buf[7-i] = c;
	}
	buf[8] = '\0';
	uart_write(buf);
}
