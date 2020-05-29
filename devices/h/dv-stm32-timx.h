/*  dv-stm32-timx.h
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
#ifndef DV_STM32_TIMX_H
#define DV_STM32_TIMX_H

#include "dv-devices.h"

/* ===== TIMx ===== */

/* All the TIMx peripherals on STM32F10x appear to have the same layout, but some
 * of the registers only exist on a subset of the modules.
 *
 *						Timx
 *	Offset	Reg			1		2..5	6..7	8		9		10..11	12		13..14
 *	0x00	cr[0]		x		x		x		x		x		x		x		x
 *	0x04	cr[1]		x		x		x		x
 *	0x08	smcr		x		x				x		x		x		x		x
 *	0x0c	dier		x		x		x		x		x		x		x		x
 *	0x10	sr			x		x		x		x		x		x		x		x
 *	0x14	egr			x		x		x		x		x		x		x		x
 *	0x18	ccmr[0]		x		x				x		x		x		x		x
 *	0x1c	ccmr[1]		x		x				x
 *	0x20	ccer		x		x				x		x		x		x		x
 *	0x24	cnt			x		x		x		x		x		x		x		x
 *	0x28	psc			x		x		x		x		x		x		x		x
 *	0x2c	arr			x		x		x		x		x		x		x		x
 *	0x30	rcr			x						x
 *	0x34	ccr[0]		x		x				x		x		x		x		x
 *	0x38	ccr[1]		x		x				x		x				x
 *	0x3c	ccr[2]		x		x				x
 *	0x40	ccr[3]		x		x				x
 *	0x44	bdtr		x						x
 *	0x48	dcr			x		x				x
 *	0x4c	dmar		x		x				x
 *
 * All derivatives appear to have TIM1 to TIM5.
 * TODO: add more base addresses, structures and descriptors as needed.
*/

typedef struct dv_timx_s dv_timx_t;

struct dv_timx_s
{
	dv_reg32_t cr[2];		/* Control registers CR1..2 */
	dv_reg32_t smcr;		/* Slave mode control register */
	dv_reg32_t dier;		/* DMA/interrupt enable register */
	dv_reg32_t sr;			/* Status register */
	dv_reg32_t egr;			/* Event generation register */
	dv_reg32_t ccmr[2];		/* Capture/compare mode register CCMR1..2 */
	dv_reg32_t ccer;		/* Capture/compare enable register */
	dv_reg32_t cnt;			/* Counter */
	dv_reg32_t psc;			/* Prescaler */
	dv_reg32_t arr;			/* Auto-reload register */
	dv_reg32_t rcr;			/* Repetition counter register (TIM1 and TIM8) */
	dv_reg32_t ccr[4];		/* Capture/compare registers CCR1..4 */
	dv_reg32_t bdtr;		/* Break and dead-time register (TIM1 and TIM8) */
	dv_reg32_t dcr;			/* DMA control register */
	dv_reg32_t dmar;		/* DMA address for full transfer */
};

#define DV_NTIM			5

#define DV_TIM1_BASE	0x40012C00 
#define DV_TIM2_BASE	0x40000000
#define DV_TIM3_BASE	0x40000400
#define DV_TIM4_BASE	0x40000800
#define DV_TIM5_BASE	0x40000C00
#define DV_TIM6_BASE	0x40001000
#define DV_TIM7_BASE	0x40001400

#define dv_tim1			((dv_timx_t *)DV_TIM1_BASE)[0]
#define dv_tim2			((dv_timx_t *)DV_TIM2_BASE)[0]
#define dv_tim3			((dv_timx_t *)DV_TIM3_BASE)[0]
#define dv_tim4			((dv_timx_t *)DV_TIM4_BASE)[0]
#define dv_tim5			((dv_timx_t *)DV_TIM5_BASE)[0]

typedef struct dv_timxdescr_s dv_timxdescr_t;

struct dv_timxdescr_s
{
	dv_timx_t	*addr;
	dv_reg32_t	*apben;
	dv_u32_t	mask;
};

#define DV_TIM1_DESCR	{ &dv_tim1,	&dv_rcc.apb2en, DV_RCC_TIM1	}
#define DV_TIM2_DESCR	{ &dv_tim2,	&dv_rcc.apb1en, DV_RCC_TIM2	}
#define DV_TIM3_DESCR	{ &dv_tim3,	&dv_rcc.apb1en, DV_RCC_TIM3	}
#define DV_TIM4_DESCR	{ &dv_tim4,	&dv_rcc.apb1en, DV_RCC_TIM4	}
#define DV_TIM5_DESCR	{ &dv_tim5,	&dv_rcc.apb1en, DV_RCC_TIM5	}

#define DV_TIMX_LIST \
	DV_TIM1_DESCR,	\
	DV_TIM2_DESCR,	\
	DV_TIM3_DESCR,	\
	DV_TIM4_DESCR,	\
	DV_TIM5_DESCR

/* ===== CR1 (cr[0]) ===== */
#define DV_TIM_CKD		0x300		/* Clock division CK_INT to sampling clock */
#define DV_TIM_ARPE		0x080		/* Auto-reload preload mode */
#define DV_TIM_CMS		0x060		/* Centre-aligned modes */
#define DV_TIM_DIR		0x010		/* Direction: 1 ==> down */
#define DV_TIM_OPM		0x008		/* One-pulse mode */
#define DV_TIM_URS		0x004		/* Update request source */
#define DV_TIM_UDIS		0x002		/* Update disable */
#define DV_TIM_CEN		0x001		/* Counter enable */

/* ===== CR2 (cr[1]) ===== */
#define DV_TIM_TI1		0x080		/* TI1 selection */
#define DV_TIM_MMS		0x070		/* Master mode selection */
#define DV_TIM_CCDS		0x008		/* Capture/compare DMA selection */

/* ===== SMCR ===== */
/* TODO when needed */

/* ===== DIER ===== */
#define DV_TIM_TDE		0x4000		/* Trigger DMA request enable */
#define DV_TIM_CC4DE	0x1000		/* Capture/compare 4 DMA request enable */
#define DV_TIM_CC3DE	0x0800		/* Capture/compare 3 DMA request enable */
#define DV_TIM_CC2DE	0x0400		/* Capture/compare 2 DMA request enable */
#define DV_TIM_CC1DE	0x0200		/* Capture/compare 1 DMA request enable */
#define DV_TIM_UDE		0x0100		/* Update DMA request enable */
#define DV_TIM_TIE		0x0040		/* Trigger interrupt enable */
#define DV_TIM_CC4IE	0x0010		/* Capture/compare 4 interrupt enable */
#define DV_TIM_CC3IE	0x0008		/* Capture/compare 3 interrupt enable */
#define DV_TIM_CC2IE	0x0004		/* Capture/compare 2 interrupt enable */
#define DV_TIM_CC1IE	0x0002		/* Capture/compare 1 interrupt enable */
#define DV_TIM_UIE		0x0001		/* Update interrupt enable */

/* ===== SR ===== */
#define DV_TIM_CC4OF	0x1000		/* Capture/compare 4 overcapture flag */
#define DV_TIM_CC3OF	0x0800		/* Capture/compare 3 overcapture flag */
#define DV_TIM_CC2OF	0x0400		/* Capture/compare 2 overcapture flag */
#define DV_TIM_CC1OF	0x0200		/* Capture/compare 1 overcapture flag */
#define DV_TIM_TIF		0x0040		/* Trigger interrupt flag */
#define DV_TIM_CC4IF	0x0010		/* Capture/compare 4 interrupt flag */
#define DV_TIM_CC3IF	0x0008		/* Capture/compare 3 interrupt flag */
#define DV_TIM_CC2IF	0x0004		/* Capture/compare 2 interrupt flag */
#define DV_TIM_CC1IF	0x0002		/* Capture/compare 1 interrupt flag */
#define DV_TIM_UIF		0x0001		/* Update interrupt flag */

/* ===== EGR ===== */
#define DV_TIM_TG		0x0040		/* Trigger generation */
#define DV_TIM_CC4G		0x0010		/* Capture/compare 4 generation */
#define DV_TIM_CC3G		0x0008		/* Capture/compare 3 generation */
#define DV_TIM_CC2G		0x0004		/* Capture/compare 2 generation */
#define DV_TIM_CC1G		0x0002		/* Capture/compare 1 generation */
#define DV_TIM_UG		0x0001		/* Update generation */

/* ===== CCMRx ===== */
/* These registers have duplicated bitfields in the upper and lower bytes.
 * CCMR1 bits 7..0  --> capture/compare 1
 * CCMR1 bits 15..8 --> capture/compare 2
 * CCMR2 bits 7..0  --> capture/compare 3
 * CCMR2 bits 15..8 --> capture/compare 4
 * Furthermore, some of the individual bits have different purposes, depending whether the channel is
 * in capture of compare mode.
 *
 * These macros give the bit fields for CC1/3. For CC2/4, shift left by 8.
*/
#define DV_TIM_OCxCE	0x80		/* Output compare clear enable */
#define DV_TIM_OCxM		0x70		/* Output compare mode */
#define DV_TIM_OCxPE	0x08		/* Output compare preload enable */
#define DV_TIM_OCxFE	0x04		/* Output compare fast enable */

#define DV_TIM_ICxF		0xf0		/* Input capture filter */
#define DV_TIM_ICxPSC	0x0c		/* Input capture prescaler */

#define DV_TIM_CCxS		0x03		/* Capture/compare selection */

/* ===== CCER ===== */
/* These macros give the CC1 bits. For CC2/3/4 shift left by 4/8/12 */
#define DV_TIM_CCxP		0x02		/* Channel polarity */
#define DV_TIM_CCxE		0x01		/* Channel enable */

/* ===== DCR and DMAR ===== */
/* TODO when needed */

extern const dv_timxdescr_t dv_timxdescr[DV_NTIM];

/* dv_get_timx() - get the address of the timer peripheral.
 *
 * Parameter t: timer number (1..14, depending on device)
*/
static inline dv_timx_t *dv_get_timx(int timx)
{
	return dv_timxdescr[timx-1].addr;
}

extern int dv_stm32_timx_init(int timx, unsigned prescale);

#endif
