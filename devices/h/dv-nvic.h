/*  dv-nvic.h
 *
 *  Copyright David Haworth
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
#ifndef DV_NVIC_H
#define DV_NVIC_H

#include "dv-devices.h"
#include "dv-cortex-m.h"

#ifndef DV_ARMv6_M
#define DV_ARMv6_M	0
#endif

#ifndef DV_NVIC_NLEVELBITS

#if DV_ARMv6_M
/* Always two interrupt level bits at most sigificant end of 8-bit field
*/
#define DV_NVIC_NLEVELBITS	2
#else
/* Assume 4 interrupt level bits. This is correct for STM32
*/
#define DV_NVIC_NLEVELBITS	4
#endif

#endif

#define DV_NVIC_MAXLEVEL	((0x1<<DV_NVIC_NLEVELBITS)-1)
#define DV_NVIC_LEVELSHIFT	(8-DV_NVIC_NLEVELBITS)

#if DV_ARMv6_M
/* Armv6-m doesn't provide a baseprio register so there's no way to selectively block interrupts below
 * a certain priority.
 *
 * To fake that feature we selectively mask and unmask the registers using these variables to
 * keep track of what we're doing
*/
extern dv_u32_t dv_nvic_enabled;		/* Which interrupts are enabled */
extern dv_u32_t dv_nvic_levelmask[4];	/* Which interrupts are enabled at each level. 0 is always "all" */

#endif

/* Abstract interrupt level: 0 is lowest (all enabled); DV_NVIC_MAXLEVEL is highest (all disabled)
*/
extern dv_intlevel_t dv_currentlocklevel;

/* NVIC registers.
 *
 * The gaps between the various register sets are consisent with allowing future support for
 * up to 1024 interrupt sources although the architecture document explicitly excludes
 * the possibility.
 *
 * This structure declares placeholders for all 1024 potential sources.
 *
 * In practice, the number of sources available is defined by the microcontroller implementation.
 * For example, the stm32f103 range has only 68.
*/

typedef struct dv_nvic_s dv_nvic_t;

struct dv_nvic_s
{
	dv_reg32_t iser[32];		/* Write 1 to a bit to enable an IRQ; Write 0 has no effect */
	dv_reg32_t icer[32];		/* Write 1 to a bit to disable an IRQ; Write 0 has no effect */
	dv_reg32_t ispr[32];		/* Write 1 to a bit to set an IRQ's pending flag; Write 0 has no effect */
	dv_reg32_t icpr[32];		/* Write 1 to a bit to clear an IRQ's pending flag; Write 0 has no effect */
	dv_reg32_t iabr[32];		/* Read-only. a bit at 1 indicates interrupt is active */
								/* Note: in the above register arrays, registers 16..31 are reserved */
	dv_u32_t reserved[32];
	dv_reg32_t ipr[256];		/* Interrupt priorities; 8 bits per IRQ; last 124..127 are reserved */
};

#define dv_nvic		((dv_nvic_t *)DV_NVIC_BASE)[0]

/* dv_nvic_write1() - write a 1 to a bit in an array
*/
static inline void dv_nvic_write1(dv_reg32_t bank[32], int irq)
{
	bank[irq/32] = 0x1 << (irq%32);
}

/* dv_nvic_read1() - read a bit in an array
*/
static inline int dv_nvic_read1(dv_reg32_t bank[32], int irq)
{
	return (bank[irq/32] & (0x1 << (irq%32))) != 0;
}

/* dv_nvic_enableirq() - enable an interrupt source
*/
static inline void dv_nvic_enableirq(int irq)
{
#if DV_ARMv6_M
	dv_nvic_enabled |= (0x1 << irq);
	dv_nvic.iser[0] = (dv_nvic_enabled & dv_nvic_levelmask[dv_currentlocklevel]);
#else
	dv_nvic_write1(dv_nvic.iser, irq);
#endif
}

/* dv_nvic_disableirq() - disable an interrupt source
*/
static inline void dv_nvic_disableirq(int irq)
{
#if DV_ARMv6_M
	dv_nvic_enabled &= ~(0x1 << irq);
#endif
	dv_nvic_write1(dv_nvic.icer, irq);
}

/* dv_nvic_triggerirq() - trigger an interrupt (set its pending flag)
*/
static inline void dv_nvic_triggerirq(int irq)
{
	dv_nvic_write1(dv_nvic.ispr, irq);
}

/* dv_nvic_cleaarirq() - clear an interrupt source's pending flag
*/
static inline void dv_nvic_clearirq(int irq)
{
	dv_nvic_write1(dv_nvic.icpr, irq);
}

#if DV_ARMv6_M

/* set_level_bits() - sets the software level bits for an interrupt source whose bitmask is in bit
*/
static inline void set_level_bits(dv_u32_t lvl, dv_u32_t bit)
{
	switch (lvl)
	{
	/* All cases fall through to the next
	*/
	case 3:
		dv_nvic_levelmask[3] |= bit;
	case 2:
		dv_nvic_levelmask[2] |= bit;
	case 1:
		dv_nvic_levelmask[1] |= bit;
	}
}

/* clear_level_bits() - clears the software level bits for an interrupt source whose bitmask is in bit
*/
static inline void clear_level_bits(dv_u32_t lvl, dv_u32_t bit)
{
	switch (lvl)
	{
	/* All cases fall through to the next
	*/
	case 3:
		dv_nvic_levelmask[3] &= ~bit;
	case 2:
		dv_nvic_levelmask[2] &= ~bit;
	case 1:
		dv_nvic_levelmask[1] &= ~bit;
	}
}

#endif

/* dv_nvic_setprio() - set the priority of an interrupt source
 *
 * Must be done while the IRQ is disabled.
*/
static inline void dv_nvic_setprio(int irq, dv_u8_t p)
{
	int shift = (irq%4) * 8;
	int idx = irq/4;
	dv_u32_t mask = 0xff << shift;

#if DV_ARMv6_M
	/* Assumes DV_NVIC_MAXLEVEL is 2**n - 1
	*/
	dv_u8_t oldp = (dv_nvic.ipr[idx] >> (DV_NVIC_LEVELSHIFT + shift)) & DV_NVIC_MAXLEVEL;
	dv_u32_t irqbit = 0x1 << irq;
	clear_level_bits(oldp, irqbit);
	set_level_bits(p, irqbit);
#endif

	dv_nvic.ipr[idx] = (dv_nvic.ipr[idx] & ~mask) | (((dv_u32_t)p)<<(DV_NVIC_LEVELSHIFT + shift));
}

extern void dv_nvic_init(void);


#if DV_DAVROSKA

/* Implement the davroska functions in terms of cortex M and NVIC functions.
*/
static inline void dv_init_interrupt_controller(void)
{
	dv_nvic_init();
}

static inline void dv_enable_irq(int irq)
{
	dv_nvic_enableirq(irq);
}

static inline void dv_disable_irq(int irq)
{
	dv_nvic_disableirq(irq);
}

/* dv_config_irq() - set the interrupt level of an individual interrupt source
 *
 * The lvl parameter runs from 1 (lowest) upwards. We need to limit the range.
 * Also, NVIC has inverted levels (0 is highest) so we need to reverse the order.
 * Davroska doesn't use hw level 0 so that mutexes don't block exceptions.
*/
static inline void dv_config_irq(int irq, dv_intlevel_t lvl, int unused_core)
{
	if ( lvl > DV_NVIC_MAXLEVEL )	lvl = DV_NVIC_MAXLEVEL;

	dv_printf("dv_config_irq() - irq %d, lvl %d, set to %d\n", irq, lvl, ((DV_NVIC_MAXLEVEL+1)-lvl));
	dv_nvic_setprio(irq, (dv_u8_t)((DV_NVIC_MAXLEVEL+1)-lvl));
}

/* In the davroska world, 0 is the lowest priority (enables all interrupts).
 * Interrupt sources have priorities from 1 upwards.
 * The various functions here map this onto the inverted Arm scheme
*/
static inline dv_intlevel_t dv_setirqlevel(dv_intlevel_t lvl)
{
	if ( lvl > DV_NVIC_MAXLEVEL )  lvl = DV_NVIC_MAXLEVEL;
	if ( dv_currentlocklevel == lvl )
		return lvl;

	dv_intstatus_t is = dv_disable();
	dv_intlevel_t old = dv_currentlocklevel;
	dv_currentlocklevel = lvl;

#if DV_ARMv6_M
/* ARMv6-m doesn't provide the baseprio register. We have to manage interrupt levels
 * by flipping enable bits
*/
	if ( lvl > old )
		dv_nvic.iser[0] = (dv_nvic_enabled & dv_nvic_levelmask[dv_currentlocklevel]);
	else
		dv_nvic.icer[0] = ~(dv_nvic_enabled & dv_nvic_levelmask[dv_currentlocklevel]);
#if 0
	dv_printf(" nvic enabled 0x%08x\n", dv_nvic.iser[0]);
#endif
#else
	if ( lvl == 0 )
		dv_set_basepri(0x0);
	else
		dv_set_basepri(((unsigned)((DV_NVIC_MAXLEVEL + 1) - lvl)) << DV_NVIC_LEVELSHIFT);
#endif

	dv_restore(is);
	return old;
}


#endif

#endif
