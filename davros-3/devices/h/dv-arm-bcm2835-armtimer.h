/*  dv-arm-bcm2835-armtimer.h - ARM timer on bcm2835 (raspberry pi)
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
#ifndef dv_arm_bcm2835_armtimer_h
#define dv_arm_bcm2835_armtimer_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#ifndef DV_PBASE
#error	"No definition of DV_PBASE in the board headers. Please fix!"
#endif

/* From BCM2835-ARM-Peripherals.pdf:
 *
 * The ARM Timer is based on a ARM AP804, but it has a number of differences with the standard
 * SP804:
 *    * There is only one timer.
 *    * It only runs in continuous mode.
 *    * It has a extra clock pre-divider register.
 *    * It has a extra stop-in-debug-mode control bit.
 *    * It also has a 32-bit free running counter.
 * The clock from the ARM timer is derived from the system clock. This clock can change dynamically
 * e.g. if the system goes into reduced power or in low power mode. Thus the clock speed adapts to
 * the overal system performance capabilities. For accurate timing it is recommended to use the
 * system timers.
*/

typedef struct dv_arm_bcm2835_armtimer_s dv_arm_bcm2835_armtimer_t;

struct dv_arm_bcm2835_armtimer_s
{
	dv_reg32_t load;
	dv_reg32_t value;			/* Read only */
	dv_reg32_t control;
	dv_reg32_t irq_clr_ack;		/* "Write only" Read gives 0x544d5241 .. "ARMT" reversed. */
	dv_reg32_t raw_irq;			/* Read only */
	dv_reg32_t masked_irq;		/* Read only */
	dv_reg32_t reload;
	dv_reg32_t predivider;		/* Not in real 804! */
	dv_reg32_t frc;				/* Not in real 804! */
};

#define dv_arm_bcm2835_armtimer	((dv_arm_bcm2835_armtimer_t *)(DV_PBASE+0x00b400))[0]

/* Bits in control register.
*/
#define DV_FRC_PRESCALE		0x00ff0000	/* Frc freq is sys_clk/(prescale+1). Not in standard 804 timer! */
#define DV_FRC_ENABLE		0x00000200	/* 1 = frc enabled. Not in standard 804 timer! */
#define DV_FRC_FREEZE		0x00000100	/* 1 = halt in debug mode. Not in standard 804 timer! */
#define DV_TMR_ENABLE		0x00000080	/* 1 = timer enabled */
#define DV_TMR_PERIODIC		0x00000040	/* 1 = periodic timer (not supported in BCM2835 - should be 0!)*/
#define DV_TMR_IRQ_ENABLE	0x00000020	/* 1 = timer interrupt enabled */
#define DV_TMR_PRESCALE		0x0000000c	/* 00 = tmr_clk/11, 01 = tmr_clk/16, 10 = tmr_clk/256. */
#define DV_TMR_LEN			0x00000002	/* 0 = 16-bit, 1 = 23-bit */
#define DV_TMR_ONESHOT		0x00000001	/* 1 = one-shot. (not supported in BCM2835 - should be 0!) */

/* Bits in raw_irq and masked_irq.
*/
#define DV_IRQ_PENDING		0x00000001	/* 1 = IRQ is pending/asserted */

/* Bits in predivider.
*/
#define DV_PREDIV			0x000003ff	/* Predivider value. tmr_clk = sys_clk/(prediv+1) */

/* dv_arm_bcm2835_armtimer_set_frc_prescale() - set FRC prescaler.
 *
 * Parameter is the true prescaler value, range 1..256 ; e.g. to divide by 250, pass 250 (not 249).
*/
static inline void dv_arm_bcm2835_armtimer_set_frc_prescale(dv_u32_t pre)
{
	pre = (pre-1)<<16;
	if ( (pre & ~DV_FRC_PRESCALE) == 0 )	/* Ensure no extraneous bits. */
	{
		dv_arm_bcm2835_armtimer.control = (dv_arm_bcm2835_armtimer.control & ~DV_FRC_PRESCALE) | pre;
	}
}

/* dv_arm_bcm2835_armtimer_enable_frc() - enable the FRC.
*/
static inline void dv_arm_bcm2835_armtimer_enable_frc(void)
{
	dv_arm_bcm2835_armtimer.control |= DV_FRC_ENABLE;
}

/* dv_arm_bcm2835_armtimer_read_frc() - read the FRC.
*/
static inline dv_u32_t dv_arm_bcm2835_armtimer_read_frc(void)
{
	return dv_arm_bcm2835_armtimer.frc;
}

#endif
