/* dv-arm-gic.h - generic interrupt controller header file for davros
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
#ifndef dv_arm_gic_h
#define dv_arm_gic_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <cpufamily/arm/h/dv-arm-configbase.h>

#if !DV_ASM

typedef struct dv_gicc_s dv_gicc_t;
typedef struct dv_gicd_s dv_gicd_t;

/* Interrupt controller (banked, one per core, at same address)
*/
struct dv_gicc_s
{
	dv_reg32_t iccicr;		/* CPU interface control register */
	dv_reg32_t iccpmr;		/* Interrupt priority mask register */
	dv_reg32_t iccbpr;		/* Binary point register */
	dv_reg32_t icciar;		/* Interrupt acknowledge register */
	dv_reg32_t icceoir;		/* End of interrupt register */
	dv_reg32_t iccrpr;		/* Running priority register */
	dv_reg32_t icchpir;		/* Highest pending interrupt register */
	dv_reg32_t iccabpr;		/* Aliased non-secure binary point register */
};

/* Interrupt distributor (non-banked)
*/
struct dv_gicd_s
{
	dv_reg32_t icdctlr;				/* Distributor control register. */
	dv_reg32_t icdtyper;			/* Interrupt controller type Register. */
	dv_reg32_t icdiidr;				/* Distributor implementer identification register. */
	dv_reg32_t gap0[5];
	dv_reg32_t icdimpl0[8];			/* Implementation defined registers. */
	dv_reg32_t gap1[16];
	dv_reg32_t icdigroupr[32];		/* Interrupt group registers. */
	dv_reg32_t icdisenabler[32];	/* Interrupt set-enable registers. */
	dv_reg32_t icdicenabler[32];	/* Interrupt clear-enable registers. */
	dv_reg32_t icdispendr[32];		/* Interrupt set-pending registers. */
	dv_reg32_t icdicpendr[32];		/* Interrupt clear-pending registers. */
	dv_reg32_t icdisactiver[32];	/* Interrupt set-active registers. */
	dv_reg32_t icdicactiver[32];	/* Interrupt clear-active registers. */
	dv_reg8_t  icdipriorityr[1020];	/* Interrupt priority registers. */
	dv_reg32_t gap2[1];
	dv_reg8_t  icditargetsr[1020];	/* Interrupt processor targets registers. */
	dv_reg32_t gap3[1];
	dv_reg32_t icdicfgr[64];		/* Interrupt configuration registers. */
	dv_reg32_t icdimpl1[64];		/* Implementation defined registers. */
	dv_reg32_t icdnsacr[64];		/* Non-secure access control registers (optional). */
	dv_reg32_t icdsgir;				/* Software generated interrupt register. */
	dv_reg32_t gap4[3];
	dv_reg8_t  icdcpendsgir[16];	/* SGI clear-pending registers. */
	dv_reg8_t  icdspendsgir[16];	/* SGI set-pending registers. */
};

/* GIC functions
*/
void dv_init_gic(void);

static inline void dv_config_irq(int index, int core, int prio)
{
	dv_gicd_t *icd = dv_get_config_base(DV_GICD_OFFSET);
	icd->icdipriorityr[index] = prio;
	icd->icditargetsr[index] = (1 << core);
}

static inline void dv_enable_irq(int index)
{
	dv_gicd_t *icd = dv_get_config_base(DV_GICD_OFFSET);
	icd->icdisenabler[index/32] = (1 << (index % 32));
}

static inline void dv_disable_irq(int index)
{
	dv_gicd_t *icd = dv_get_config_base(DV_GICD_OFFSET);
	icd->icdicenabler[index/32] = (1 << (index % 32));
}

static inline void dv_set_level(int lvl)
{
	dv_gicc_t *icc = dv_get_config_base(DV_GICC_OFFSET);
	icc->iccpmr = lvl;
}

#endif

#define DV_ICCICR_ENABLE		0x1		/* Enable bit in iccicr	*/
#define DV_ICDCTLR_ENABLE		0x1		/* Enable bit in icdctlr	*/

/* Bit fields in icdsgir (software-generated interrupt register).
*/
#define DV_ICDSGIR_SGINTID		0x0000000fu		/* Interrupt ID, 0..15 */
#define DV_ICDSGIR_NSATT		0x00008000u		/* Security attribute */
#define DV_ICDSGIR_TARGETS		0x00ff0000u		/* Target core list ... */
#define DV_ICDSGIR_CORE0		0x00010000u		/* ... Target core 0 */
#define DV_ICDSGIR_CORE1		0x00020000u		/* ... Target core 1 */
#define DV_ICDSGIR_CORE2		0x00040000u		/* ... Target core 2 */
#define DV_ICDSGIR_CORE3		0x00080000u		/* ... Target core 3 */
#define DV_ICDSGIR_CORE4		0x00100000u		/* ... Target core 4 */
#define DV_ICDSGIR_CORE5		0x00200000u		/* ... Target core 5 */
#define DV_ICDSGIR_CORE6		0x00400000u		/* ... Target core 6 */
#define DV_ICDSGIR_CORE7		0x00800000u		/* ... Target core 7 */
#define DV_ICDSGIR_FILTER		0x03000000u		/* Target list filter ... */
#define DV_ICDSGIR_USELIST		0x00000000u		/* ... send interrupt to all cores in target list */
#define DV_ICDSGIR_ALLBUTME		0x01000000u		/* ... send to all cores except the requesting core */
#define DV_ICDSGIR_ONLYME		0x02000000u		/* ... send to the requesting core only */

/* Bit fields in icciar (interrupt acknowledge register)
*/
#define DV_ICCIAR_INTERRUPT_ID	0x3ffu;
#define DV_ICCIAR_CPUID			0x1c00u;

/* Special interrupt IDs
*/
#define DV_IID_SGI0			0		/* GIC software-generated interrupts */
#define DV_IID_SGI1			1
#define DV_IID_SGI2			2
#define DV_IID_SGI3			3
#define DV_IID_SGI4			4
#define DV_IID_SGI5			5
#define DV_IID_SGI6			6
#define DV_IID_SGI7			7
#define DV_IID_SGI8			8
#define DV_IID_SGI9			9
#define DV_IID_SGI10		10
#define DV_IID_SGI11		11
#define DV_IID_SGI12		12
#define DV_IID_SGI13		13
#define DV_IID_SGI14		14
#define DV_IID_SGI15		15

#define DV_IID_GTIMER		27		/* Global timer */
#define DV_IID_FIQ			28		/* Legacy FIQ */
#define DV_IID_PTIMER		29		/* Private timer */
#define DV_IID_A9WDG		30		/* Cortex A9 watchdog */
#define DV_IID_IRQ			28		/* Legacy IRQ */

#define DV_IID_SPURIOUS		1023

#endif
