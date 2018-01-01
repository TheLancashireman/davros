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

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#if !DV_ASM

/* BCM2835 interrupt controller (as used on Raspberry Pi CPUs).
 *
 * The BCM2835 interrupt controller is a very simple unit. There is no vectoring, and
 * there are no priority levels. The only priority control is FIQ/IRQ. Only one source
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

#define dv_arm_bcm2835_interruptcontroller	((dv_arm_bcm2835_interruptcontroller_t *)0x2000b200)[0]

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
}

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
typedef enum dv_bcd2835_softvector_e
{
	/* IRQ0	*/
	dv_iid_syst_cm1,
	dv_iid_syst_cm3,
	dv_iid_aux,
#if 0
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
#endif
#if 0
	/* Basic */
	dv_iid_timer,
	dv_iid_mailbox,
	dv_iid_doorbell0,
	dv_iid_doorbell1,
	dv_iid_gpu0halt,
	dv_iid_gpu1halt,
	dv_iid_illegal0,
	dv_iid_illegal1,
#endif
	dv_n_iid				/* Must be last */
} dv_bcd2835_softvector_t;

typedef struct bcm2835_irq_s
{
	int idx;
	dv_u32_t mask;
} bcm2835_irq_t;

extern const bcm2835_irq_t bcm2835_irq_list[dv_n_iid];

static inline void dv_config_irq(int unused_index, int unused_core, int unused_prio)
{
	/* Nothing to do.
	*/
}

static inline void dv_enable_irq(int index)
{
	dv_arm_bcm2835_interruptcontroller.irq_enable[bcm2835_irq_list[index].idx] = bcm2835_irq_list[index].mask;
}

static inline void dv_disable_irq(int index)
{
	dv_arm_bcm2835_interruptcontroller.irq_disable[bcm2835_irq_list[index].idx] = bcm2835_irq_list[index].mask;
}

static inline void dv_set_level(int lvl)
{
	/* Nothing to do.
	*/
}




#endif

#endif
