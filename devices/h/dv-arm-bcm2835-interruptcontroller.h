/*  dv-arm-bcm2835-interruptcontroller.h - interrupt controller on bcm2835 etc. (raspberry pi)
 *
 *  Copyright 2017 David Haworth
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
#ifndef dv_arm_bcm2835_interruptcontroller_h
#define dv_arm_bcm2835_interruptcontroller_h	1

#include <dv-devices.h>

#ifndef DV_PBASE
#error	"No definition of DV_PBASE in the board headers. Please fix!"
#endif

#include <dv-stdio.h>

#ifndef DV_SUPPORT_INTLEVEL
#define DV_SUPPORT_INTLEVEL		0
#endif

#ifndef DV_DEBUG
#define DV_DEBUG 	0
#endif

#if !DV_ASM

/* BCM2835 interrupt controller (as used on Raspberry Pi CPUs).
 *
 * The BCM2835 interrupt controller is a very simple unit. There is no vectoring, and
 * there are no hardware priority levels. The only priority control is FIQ/IRQ. Only one source
 * can be routed to FIQ.
 *
 * There are 3 sets of 3 registers for control and status related to the IRQ signal.
 * Each set of 3 has a base IRQ register and two IRQ registers. Annoyingly, the
 * "pending" set has the registers in a different order from the other two sets. :-(
 *
 * Refer to the corresponding C file dv-arm-bcm2835-interruptcontroller.c for a full
 * description of the device and the Davros driver for it.
*/
typedef struct dv_arm_bcm2835_interruptcontroller_s dv_arm_bcm2835_interruptcontroller_t;

struct dv_arm_bcm2835_interruptcontroller_s
{
	dv_reg32_t basic_pending;		/* Wrong order :-(   But we can sort this out in the handler */
	dv_reg32_t irq_pending[2];
	dv_reg32_t fiq_control;
	dv_reg32_t irq_enable[3];		/* Write 1 to enable. Basic is index 2 */
	dv_reg32_t irq_disable[3];		/* Write 1 to disable. Basic is index 2 */
};

/* Data type for interrupt masks.
*/
typedef struct dv_bcm2835_imask_s dv_bcm2835_imask_t;

struct dv_bcm2835_imask_s
{	dv_u32_t mask[3];
};

#define dv_arm_bcm2835_interruptcontroller	((dv_arm_bcm2835_interruptcontroller_t *)(DV_PBASE+0x00b200))[0]

/* Bits in the "basic" registers
 * The remaining bits are used by the GPU.
*/
#define DV_INT_TIMER		0x00000001
#define DV_INT_MAILBOX		0x00000002
#define DV_INT_DOORBELL0	0x00000004
#define DV_INT_DOORBELL1	0x00000008
#define DV_INT_GPU0HALT		0x00000010
#define DV_INT_GPU1HALT		0x00000020
#define DV_INT_ILLEGAL0		0x00000040	/* Illegal access 0 */
#define DV_INT_ILLEGAL1		0x00000080	/* Illegal access 1 */

/* Bits in the irq[0] registers.
 * The remaining bits are undocumented and could be used by the GPU.
 *
 * CM1 and CM3 are also undocumented. The GPU appears to use CM0 and CM2 of the system timer
 * but leaves CM1 and CM3 alone. In "bare metal", it was discovered by experiment that
 * CM1 and CM3 are bits 1 and 3. Assume then that bits 0 and 2 are CM0 and 2.
*/
#define DV_INT_SYST_CM1		0x00000002
#define DV_INT_SYST_CM3		0x00000008
#define DV_INT_AUX			0x20000000

/* Bits in the irq[1] registers
 * The remaining bits are undocumented and could be used by the GPU.
*/
#define DV_INT_I2CSPI_SLV	0x00000800
#define DV_INT_PWA0			0x00002000
#define DV_INT_PWA1			0x00004000
#define DV_INT_SMI			0x00010000
#define DV_INT_GPIO0		0x00020000
#define DV_INT_GPIO1		0x00040000
#define DV_INT_GPIO2		0x00080000
#define DV_INT_GPIO3		0x00100000
#define DV_INT_I2C			0x00200000
#define DV_INT_SPI			0x00400000
#define DV_INT_PCM			0x00800000
#define DV_INT_UART			0x02000000

/* Interrupt identifiers.
 *
 * There are no banked interrupts, so no. of IIDs is the same as no. of software vectors.
 *
 * The SoC does not define any vector numbers, so we define them here.
*/
typedef enum dv_irqid_e
{
	/* IRQ0	*/
	dv_iid_syst_cm1,
	dv_iid_syst_cm3,
	dv_iid_aux,
	/* IRQ1 */
	dv_iid_i2cspi_slv,
	dv_iid_pwa0,
	dv_iid_pwa1,
	dv_iid_smi,
	dv_iid_gpio0,
	dv_iid_gpio1,
	dv_iid_gpio2,
	dv_iid_gpio3,
	dv_iid_i2c,
	dv_iid_spi,
	dv_iid_pcm,
	dv_iid_uart,
	/* Basic */
	dv_iid_timer,
	dv_iid_mailbox,
	dv_iid_doorbell0,
	dv_iid_doorbell1,
	dv_iid_gpu0halt,
	dv_iid_gpu1halt,
	dv_iid_illegal0,
	dv_iid_illegal1,
	dv_n_bcm2835_iid				/* Must be last */
} dv_irqid_t;

typedef struct dv_bcm2835_irq_s
{
	int idx;
	dv_u32_t mask;
} dv_bcm2835_irq_t;

typedef signed char dv_intlevel_t;

extern const dv_bcm2835_irq_t dv_bcm2835_irq_list[];

#if DV_SUPPORT_INTLEVEL
/* Support for artifical interrupt levels...
 * Current level runs from 0 (all enabled) to 8 (all disabled)
 * An individual IRQ can have a level from -1 (default; not configured) to 7.
 * Comparison for enabling is "irq-level >= current-level"
*/
extern dv_intlevel_t dv_bcm2835_irq_level[dv_n_bcm2835_iid];
extern dv_intlevel_t dv_currentlocklevel;
extern dv_bcm2835_imask_t dv_bcm2835_irq_enabled;
extern dv_bcm2835_imask_t dv_bcm2835_levelmasks[9];
#endif

/* dv_config_irq() - configure an interrupt request
 *
 * If artificial interrupt levels are supported, set the level
 * otherwise nothing to do
*/
#if DV_SUPPORT_INTLEVEL
extern void dv_config_irq(dv_irqid_t index, dv_intlevel_t level, int unused_core);
#else
static inline void dv_config_irq(dv_irqid_t unused_index, dv_intlevel_t unused_level, int unused_core)
{
	/* Nothing to do
	*/
}
#endif

/* dv_enable_irq() - enable an interrupt request
 *
 * If artificial interrupt levels are supported, set the irq's bit in the global mask register and
 * only enable if the irq's level is high enough
 * Without levels: just enable the irq.
*/
static inline void dv_enable_irq(dv_irqid_t index)
{
	int reg = dv_bcm2835_irq_list[index].idx;
	dv_u32_t mask = dv_bcm2835_irq_list[index].mask;

#if DV_SUPPORT_INTLEVEL
	dv_bcm2835_irq_enabled.mask[reg] |= mask;

#if DV_DEBUG
	dv_printf("dv_enable_irq() - dv_bcm2835_irq_enabled.mask[%d] set to 0x%08x, irqlevel %d, current %d\n",
							reg, dv_bcm2835_irq_enabled.mask[reg], dv_bcm2835_irq_level[index], dv_currentlocklevel);
#endif

	if ( dv_bcm2835_irq_level[index] >= dv_currentlocklevel )
	{
#if DV_DEBUG
		dv_printf("dv_enable_irq() - writing 0x%08x to .irq_enable[%d]\n", mask, reg);
#endif
		dv_arm_bcm2835_interruptcontroller.irq_enable[reg] = mask;	/* Register is "write 1 to enable" */
	}
#else
	dv_arm_bcm2835_interruptcontroller.irq_enable[reg] = mask;	/* Register is "write 1 to enable" */
#endif
}

/* dv_disable_irq() - disable an interrupt request
 *
 * If artificial interrupt levels are supported, clear the irq's bit in the global mask register
 * With or without levels: disable the irq.
*/
static inline void dv_disable_irq(dv_irqid_t index)
{
	int reg = dv_bcm2835_irq_list[index].idx;
	dv_u32_t mask = ~dv_bcm2835_irq_list[index].mask;

#if DV_SUPPORT_INTLEVEL
	dv_bcm2835_irq_enabled.mask[reg] &= ~mask;
#endif
	dv_arm_bcm2835_interruptcontroller.irq_disable[reg] = mask;		/* Register is "write 1 to disable" */
}

/* dv_setirqlevel() - sets the current IRQ lock level
 *
 * If artificial levels are not supported, this function does nothing
*/
static inline dv_intlevel_t dv_setirqlevel(dv_intlevel_t lvl)
{
#if DV_SUPPORT_INTLEVEL
	if ( lvl < 0 )	lvl = 0;
	if ( lvl > 8 ) 	lvl = 8;
	if ( dv_currentlocklevel == lvl )
		return lvl;

	dv_intstatus_t is = dv_disable();

	dv_intlevel_t old = dv_currentlocklevel;
	dv_currentlocklevel = lvl;

	for ( int i = 0; i < 3; i++ )
	{
		dv_arm_bcm2835_interruptcontroller.irq_disable[i] = 0xffffffff;
		dv_arm_bcm2835_interruptcontroller.irq_enable[i] =
				dv_bcm2835_irq_enabled.mask[i] & dv_bcm2835_levelmasks[lvl].mask[i];
	}

	dv_restore(is);
	return old;
#else
	return 0;
#endif
}

static inline void dv_init_interrupt_controller(void)
{
	/* Disable all interrupt sources, including those used by the GPU.
	*/
	dv_arm_bcm2835_interruptcontroller.irq_disable[0] = 0xffffffff;
	dv_arm_bcm2835_interruptcontroller.irq_disable[1] = 0xffffffff;
	dv_arm_bcm2835_interruptcontroller.irq_disable[2] = 0xffffffff;

	/* Disable FIQ.
	*/
	dv_arm_bcm2835_interruptcontroller.fiq_control = 0;

#if DV_SUPPORT_INTLEVEL
	/* Initialise the configured levels of all interrupt sources
	*/
	for ( int i = 0; i < dv_n_bcm2835_iid; i++ )
		dv_bcm2835_irq_level[i] = -1;
#endif
}

#if DV_DAVROSKA

void dv_bcm2835_interrupt_handler(void);

#else

void dv_bcm2835_interrupt_handler(dv_kernel_t *kvars);

#endif

#endif

#endif
