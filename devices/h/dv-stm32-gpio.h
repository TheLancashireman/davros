/*  dv-stm32-gpio.h
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
#ifndef DV_STM32_GPIO_H
#define DV_STM32_GPIO_H

#include "dv-devices.h"

/* ===== GPIO ===== */

typedef struct dv_gpio_s dv_gpio_t;

struct dv_gpio_s
{
	dv_reg32_t cr[2];		/* Control registers - 4 bits per pin; pins 0..7 in cr[0], pins 8..15 in cr[1] */
	dv_reg32_t idr;			/* Input data */
	dv_reg32_t odr;			/* Output data */
	dv_reg32_t bsrr;		/* Bit set/reset - write 1 to set or clear pin; upper half is clear, lower half set */
	dv_reg32_t brr;			/* Bit reset - write 1 to clear pin */
	dv_reg32_t lock;		/* Configuration lock */
};

#define DV_GPIO_BASE		0x40010800

#define DV_GPIOA_BASE		(DV_GPIO_BASE + 0x0000)
#define DV_GPIOB_BASE		(DV_GPIO_BASE + 0x0400)
#define DV_GPIOC_BASE		(DV_GPIO_BASE + 0x0800)
#define DV_GPIOD_BASE		(DV_GPIO_BASE + 0x0c00)
#define DV_GPIOE_BASE		(DV_GPIO_BASE + 0x1000)
#define DV_GPIOF_BASE		(DV_GPIO_BASE + 0x1400)
#define DV_GPIOG_BASE		(DV_GPIO_BASE + 0x1800)

#define dv_gpio_a			((dv_gpio_t *)DV_GPIOA_BASE)[0]
#define dv_gpio_b			((dv_gpio_t *)DV_GPIOB_BASE)[0]
#define dv_gpio_c			((dv_gpio_t *)DV_GPIOC_BASE)[0]

#define DV_GPIO_IN_ANALOG	(0x0 | 0x0)
#define DV_GPIO_IN_FLOAT	(0x4 | 0x0)
#define DV_GPIO_IN_PUD		(0x8 | 0x0)		/* Pullup/pulldown selected by setting odr bit to 1/0 */
#define DV_GPIO_IN_RES		(0xc | 0x0)		/* Reserved; don't use */

#define DV_GPIO_OUT_PP_10	(0x0 | 0x1)
#define DV_GPIO_OUT_OD_10	(0x4 | 0x1)
#define DV_GPIO_ALT_PP_10	(0x8 | 0x1)
#define DV_GPIO_ALT_OD_10	(0xc | 0x1)

#define DV_GPIO_OUT_PP_2	(0x0 | 0x2)
#define DV_GPIO_OUT_OD_2	(0x4 | 0x2)
#define DV_GPIO_ALT_PP_2	(0x8 | 0x2)
#define DV_GPIO_ALT_OD_2	(0xc | 0x2)

#define DV_GPIO_OUT_PP_50	(0x0 | 0x3)
#define DV_GPIO_OUT_OD_50	(0x4 | 0x3)
#define DV_GPIO_ALT_PP_50	(0x8 | 0x3)
#define DV_GPIO_ALT_OD_50	(0xc | 0x3)

/* ===== AFIO ===== */

typedef struct dv_afio_s dv_afio_t;

struct dv_afio_s
{
	dv_reg32_t	evcr;		/* Event control register */
	dv_reg32_t	mapr;		/* Remap and debug I/O configuration register */
};

/* evcr
 *
 * The lower 4 bits are the pin number (0..15) of the selected port.
*/
#define DV_EV_ENABLE		0x80		/* Enables routing the Cortex EVENTOUT signal to a port */
#define DV_EV_PORTA			0x00
#define DV_EV_PORTB			0x10
#define DV_EV_PORTC			0x20
#define DV_EV_PORTD			0x30
#define DV_EV_PORTE			0x40

#define DV_MAP_SWJ				0x07000000	/* Write-only. Writing this value has NO EFFECT */
#define DV_MAP_ADC2_ETRGREG		0x00100000
#define DV_MAP_ADC2_ETRGINJ		0x00080000
#define DV_MAP_ADC1_ETRGREG		0x00040000
#define DV_MAP_ADC1_ETRGINJ		0x00020000
#define DV_MAP_TIM5CH4			0x00010000
#define DV_MAP_PD01				0x00008000
#define DV_MAP_CAN				0x00006000
#define DV_MAP_TIM4				0x00001000
#define DV_MAP_TIM3				0x00000c00
#define DV_MAP_TIM2				0x00000300
#define DV_MAP_TIM1				0x000000c0
#define DV_MAP_USART3			0x00000030	/* Mask; see below */
#define DV_MAP_USART3_FULL		0x00000030	/* PD8-12 */
#define DV_MAP_USART3_PART		0x00000010	/* PC10-12/PB13-14 */
#define DV_MAP_USART3_NO		0x00000000	/* PB10-14 */
#define DV_MAP_USART2			0x00000008	/* 0 -> PA0-4;		1 -> PD3-7 */
#define DV_MAP_USART1			0x00000004	/* 0 -> PA9/10;		1 -> PB6/7 */
#define DV_MAP_I2C1				0x00000002	/* 0 -> PB6/7;		1 -> PB8/9 */
#define DV_MAP_SPI1				0x00000001	/* 0 -> PA4-7;		1 -> PA15/PB3-5 */

/* Functions
*/
extern void dv_stm32_gpio_pinmode(char iop, int pin, dv_u32_t mode);

#endif
