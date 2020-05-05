/*  dv-stm32-rcc.h
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
#ifndef DV_STM32_RCC_H
#define DV_STM32_RCC_H

#include "dv-devices.h"

/* ===== RCC ===== */

typedef struct dv_rcc_s dv_rcc_t;

struct dv_rcc_s
{
	dv_reg32_t cc;			/* clock control */
	dv_reg32_t cfg;			/* clock config */
	dv_reg32_t ci;			/* clock interrupt */
	dv_reg32_t apb2rst;		/* peripheral reset */
	dv_reg32_t apb1rst;		/* peripheral reset */
	dv_reg32_t ahben;		/* peripheral clock enable */
	dv_reg32_t apb2en;		/* peripheral clock enable */
	dv_reg32_t apb1en;		/* peripheral clock enable */
	dv_reg32_t bdc;			/* backup domain control */
	dv_reg32_t cs;			/* control/status */
};

#define DV_RCC_BASE			0x40021000
#define dv_rcc				((dv_rcc_t *)DV_RCC_BASE)[0]

/* CR */
#define DV_RCC_PLLRDY		0x02000000
#define DV_RCC_PLLON		0x01000000
#define DV_RCC_CSSON		0x00080000
#define DV_RCC_HSEBYP		0x00040000
#define DV_RCC_HSERDY		0x00020000
#define DV_RCC_HSEON		0x00010000
#define DV_RCC_HSICAL		0x0000ff00
#define DV_RCC_HSITRIM		0x000000f8
#define DV_RCC_HSIRDY		0x00000002
#define DV_RCC_HSION		0x00000001

/* CFGR */
#define DV_RCC_MCO			0x07000000
#define DV_RCC_USBPRE		0x00400000
#define DV_RCC_PLLMUL		0x003c0000
#define DV_RCC_PLLXTPRE		0x00020000
#define DV_RCC_PLLSRC		0x00010000
#define DV_RCC_ADCPRE		0x0000c000
#define DV_RCC_PPRE2		0x00003800
#define DV_RCC_PPRE1		0x00000700
#define DV_RCC_HPRE			0x000000f0
#define DV_RCC_SWS			0x0000000c
#define DV_RCC_SW			0x00000003

/* AHBEN */
#define DV_RCC_SDIO			0x00000400
#define DV_RCC_FSMC			0x00000100
#define DV_RCC_CRC			0x00000400
#define DV_RCC_FLITF		0x00000100
#define DV_RCC_SRAM			0x00000400
#define DV_RCC_DMA2			0x00000200
#define DV_RCC_DMA1			0x00000100

/* APB2EN and APB2RST */
#define DV_RCC_TIM11		0x00200000
#define DV_RCC_TIM10		0x00100000
#define DV_RCC_TIM9			0x00080000
#define DV_RCC_ADC3			0x00008000
#define DV_RCC_USART1		0x00004000
#define DV_RCC_TIM8			0x00002000
#define DV_RCC_SPI1			0x00001000
#define DV_RCC_TIM1			0x00000800
#define DV_RCC_ADC2			0x00000400
#define DV_RCC_ADC1			0x00000200
#define DV_RCC_IOPG			0x00000100
#define DV_RCC_IOPF			0x00000080
#define DV_RCC_IOPE			0x00000040
#define DV_RCC_IOPD			0x00000020
#define DV_RCC_IOPC			0x00000010
#define DV_RCC_IOPB			0x00000008
#define DV_RCC_IOPA			0x00000004
#define DV_RCC_AFIO			0x00000001

/* APB1EN and APB1RST */
#define DV_RCC_DAC			0x20000000
#define DV_RCC_PWR			0x10000000
#define DV_RCC_BKP			0x08000000
#define DV_RCC_CAN			0x02000000
#define DV_RCC_USB			0x00800000
#define DV_RCC_I2C2			0x00400000
#define DV_RCC_I2C1			0x00200000
#define DV_RCC_UART5		0x00100000
#define DV_RCC_UART4		0x00080000
#define DV_RCC_USART3		0x00040000
#define DV_RCC_USART2		0x00020000
#define DV_RCC_SPI3			0x00008000
#define DV_RCC_SPI2			0x00004000
#define DV_RCC_WWDG			0x00000800
#define DV_RCC_TIM14		0x00000100
#define DV_RCC_TIM13		0x00000080
#define DV_RCC_TIM12		0x00000040
#define DV_RCC_TIM7			0x00000020
#define DV_RCC_TIM6			0x00000010
#define DV_RCC_TIM5			0x00000008
#define DV_RCC_TIM4			0x00000004
#define DV_RCC_TIM3			0x00000002
#define DV_RCC_TIM2			0x00000001

/* ===== FLASH ===== */

typedef struct dv_flash_s dv_flash_t;

struct dv_flash_s
{
	dv_reg32_t ac;
	dv_reg32_t key;
	dv_reg32_t optkey;
	dv_reg32_t s;
	dv_reg32_t c;
	dv_reg32_t a;
	dv_reg32_t reserved;
	dv_reg32_t ob;
	dv_reg32_t wrp;
};

#define DV_FLASH_BASE	0x40022000
#define dv_flash		((dv_flash_t *)DV_FLASH_BASE)[0]

#define DV_FLASH_PREFETCH	0x00000010
#define DV_FLASH_HCYCLE		0x00000008
#define DV_FLASH_WAIT		0x00000007	/* Only 0, 1 or 2 valid */

extern void dv_rcc_init(void);

#endif
