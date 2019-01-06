/*  dv-arm-bcm2836.h - timers, interrupts and mailboxes on bcm2836 (raspberry pi2 and 3)
 *
 *  Copyright 2018 David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_arm_bcm2836_h
#define dv_arm_bcm2836_h	1

#include <dv-devices.h>

#ifndef DV_PBASE2
#error	"No definition of DV_PBASE2 in the board headers. Please fix!"
#endif

#if !DV_ASM

typedef struct dv_bcm2836_mb_s dv_bcm2836_mb_t;
typedef struct dv_bcm2836_s dv_bcm2836_t;

/* dv_bcm2836_mb_s - a block of mailbox registers.
 *
 * Each core has four mailboxes. Each mailbox has 32 bits.
 * There's a "write-1-to-set" register and a "read and write-1-to-clear" register for each mailbox.
 *
 * A mailbox interrupt is signalled whenever the mailbox register is non-zero.
*/
struct dv_bcm2836_mb_s
{
	dv_reg32_t mb[4];				/* Four mailbox registers (indexed by mailbox no.) */
};

/* BCM2836 ARM peripherals derived from QA7_rev3.4.pdf
 *
 * core timer:
 *	Read LS causes MS to latch.
 *	Write LS is held until write MS
*/
struct dv_bcm2836_s
{
	dv_reg32_t	control;			/* 0x00 - Control register */
	dv_reg32_t	ct_subtract;		/* 0x04 - Prescaler subtract register (deprecated, marked 'unused' in table) */
	dv_reg32_t	ct_prescaler;		/* 0x08 - Core timer prescaler */
	dv_reg32_t	gpu_routing;		/* 0x0c - GPU interrupt routing register */
	dv_reg32_t	pmu_routing_set;	/* 0x10 - PMU interrupt routing; write 1 to set */
	dv_reg32_t	pmu_routing_clr;	/* 0x14 - PMU interrupt routing; write 1 to clear */
	dv_reg32_t	unused_0;			/* 0x18 - Unused */
	dv_reg32_t	core_timer_ls;		/* 0x1c - Core timer */
	dv_reg32_t	core_timer_ms;		/* 0x20 - Core timer */
	dv_reg32_t	ltmr_routing;		/* 0x24 - Local timer interrupt routing */
	dv_reg32_t	local_routing_8_15;	/* 0x28 - Local interrupts 8-15 routing (crossed out in table, no description) */
	dv_reg32_t	axi_counters;		/* 0x2c - Axi outstanding counters */
	dv_reg32_t	axi_interrupts;		/* 0x30 - Axi outstanding interrupts */
	dv_reg32_t	ltmr_ctrl_status;	/* 0x34 - Local timer control and status */
	dv_reg32_t	ltmr_write_flags;	/* 0x38 - Local timer write flags */
	dv_reg32_t	unused_1;			/* 0x3c - Unused */
	dv_reg32_t	tmr_int_ctrl[4];	/* 0x40 - Timers interrupt control (indexed by core) */
	dv_reg32_t	mb_int_ctrl[4];		/* 0x50 - Mailboxes interrupt control (indexed by core) */
	dv_reg32_t	irq_source[4];		/* 0x60 - IRQ source control (indexed by core) */
	dv_reg32_t	fiq_source[4];		/* 0x70 - FIQ source control (indexed by core) */
	dv_bcm2836_mb_t mb_set[4];		/* 0x80 - Mailbox write 1 to set (indexed by core) */
	dv_bcm2836_mb_t mb_rd_clr[4];	/* 0xc0 - Mailbox read/write 1 to clear (indexed by core) */
};

#define dv_bcm2836_periphs	((dv_bcm2836_t *)0x40000000)[0]

/* Bits in control register
*/
#define DV_BCM2836_CT_INC2	0x0200		/* Core timer increments by 2 */
#define DV_BCM2836_CT_SRC	0x0100		/* Core timer source (0 = xtal, 1 = APB */

/* Bits in gpu_routing register
*/
#define DV_BCM2836_GPU_IRQ	0x0003		/* Specifies core number to receive GPU_IRQ */
#define DV_BCM2836_GPU_FIQ	0x000c		/* Specifies core number to receive GPU_FIQ */

/* Bits in pmu_routing_set and pmu_routing_clr registers
*/
#define DV_BCM2836_PMU_IRQ0	0x0001		/* PMU IRQ 0 enabled (provided FIQ bit is clear) */
#define DV_BCM2836_PMU_IRQ1	0x0002		/* PMU IRQ 1 enabled (provided FIQ bit is clear) */
#define DV_BCM2836_PMU_IRQ2	0x0004		/* PMU IRQ 2 enabled (provided FIQ bit is clear) */
#define DV_BCM2836_PMU_IRQ3	0x0008		/* PMU IRQ 3 enabled (provided FIQ bit is clear) */
#define DV_BCM2836_PMU_FIQ0	0x0010		/* PMU IRQ 0 goes to FIQ (overrides IRQ0 bit) */
#define DV_BCM2836_PMU_FIQ1	0x0020		/* PMU IRQ 1 goes to FIQ (overrides IRQ1 bit) */
#define DV_BCM2836_PMU_FIQ2	0x0040		/* PMU IRQ 2 goes to FIQ (overrides IRQ2 bit) */
#define DV_BCM2836_PMU_FIQ3	0x0080		/* PMU IRQ 3 goes to FIQ (overrides IRQ3 bit) */

/* Bits in ltmr_routing
*/
#define DV_BCM2836_LTR_CORE	0x0003		/* Core to which interrupt is routed */
#define DV_BCM2836_LTR_FIQ	0x0004		/* Interrupt is routed to FIQ of selected core (0 for IRQ) */

/* Bits in ltmr_ctrl_status and ltmr_write_flags register
*/
#define DV_BCM2836_LTCS_INT		0x80000000	/* Interrupt flag (read only) */
#define DV_BCM2836_LTCS_IE		0x20000000	/* Interrupt enabled */
#define DV_BCM2836_LTCS_TE		0x10000000	/* Timer enabled */
#define DV_BCM2836_LTCS_RL		0x0fffffff	/* Timer reload value */
#define DV_BCM2836_LRWF_IC		0x80000000	/* Clear interrupt (write 1 to clear interrupt) */
#define DV_BCM2836_LRWF_RL		0x40000000	/* Reload timer (write 1 to reload. No interrupt generated) */

/* Bits in tmr_int_ctrl registers
*/
#define DV_BCM2836_CNTPS_IRQ	0x0001	/* CNTPS IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_CNTPNS_IRQ	0x0002	/* CNTPNS IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_CNTHP_IRQ	0x0004	/* CNTHP IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_CNTV_IRQ		0x0008	/* CNTV IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_CNTPS_FIQ	0x0010	/* CNTPS FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_CNTPNS_FIQ	0x0020	/* CNTPNS FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_CNTHP_FIQ	0x0040	/* CNTHP FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_CNTV_FIQ		0x0080	/* CNTV FIQ enabled (overrides IRQ bit) */

/* Bits in mb_int_ctrl registers
*/
#define DV_BCM2836_MB0_IRQ		0x0001	/* Mailbox 0 IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_MB1_IRQ		0x0002	/* Mailbox 1 IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_MB2_IRQ		0x0004	/* Mailbox 2 IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_MB3_IRQ		0x0008	/* Mailbox 3 IRQ enabled (provided FIQ bit is clear) */
#define DV_BCM2836_MB0_FIQ		0x0001	/* Mailbox 0 FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_MB1_FIQ		0x0002	/* Mailbox 1 FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_MB2_FIQ		0x0004	/* Mailbox 2 FIQ enabled (overrides IRQ bit) */
#define DV_BCM2836_MB3_FIQ		0x0008	/* Mailbox 3 FIQ enabled (overrides IRQ bit) */

/* Axi counter/interrupt registers
*/
#define DV_BCM2836_AXI_CT_WR	0x03ff0000	/* Bits 25..16 - outstanding writes */
#define DV_BCM2836_AXI_CT_RD	0x000003ff	/* Bits 9..0 - outstanding reads */
#define	DV_BCM2836_AXI_IRQ_IE	0x00100000	/* Bit 20 - Axi interrupt enable (hard-wired to core 0) */
#define	DV_BCM2836_AXI_IRQ_TO	0x000fffff	/* Bits 19..0 - Axi time-outÂMS24 bits */

/* Bits in irq_source and fiq_source
 *
 * According to this document, the "GPU" interrupt is actually the BCM2835 interrupt controller.
 * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson03/linux/interrupt_controllers.md
*/
#define DV_BCM2836_SRC_CNTPS	0x0001	/* CNTPS interrupt requested */
#define DV_BCM2836_SRC_CNTPNS	0x0002	/* CNTPNS interrupt requested */
#define DV_BCM2836_SRC_CNTHP	0x0004	/* CNTHP interrupt requested */
#define DV_BCM2836_SRC_CNTV		0x0008	/* CNTV interrupt requested */
#define DV_BCM2836_SRC_MB0		0x0010	/* Mailbox 0 interrupt requested */
#define DV_BCM2836_SRC_MB1		0x0020	/* Mailbox 1 interrupt requested */
#define DV_BCM2836_SRC_MB2		0x0040	/* Mailbox 2 interrupt requested */
#define DV_BCM2836_SRC_MB3		0x0080	/* Mailbox 3 interrupt requested */
#define DV_BCM2836_SRC_GPU		0x0100	/* GPU interrupt requested (see note above) */
#define DV_BCM2836_SRC_PMU		0x0200	/* PMU interrupt requested */
#define DV_BCM2836_SRC_AXI		0x0400	/* AXI interrupt requested */
#define DV_BCM2836_SRC_LT		0x0800	/* Local timer interrupt requested */
#define DV_BCM2836_SRC_PERIPH	0x3f000	/* Peripheral interrupt(s) requested (currently not used) */

#endif


#endif
