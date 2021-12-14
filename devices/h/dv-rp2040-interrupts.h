/*  dv-rp2040-interrupts.h
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
#ifndef DV_RP2040_INTERRUPTS_H
#define DV_RP2040_INTERRUPTS_H

#include "dv-devices.h"

enum dv_irqid_e
{
	dv_irq_wwdg,			/* 00 - window watchdog interrupt */
	dv_irq_pvd,				/* 01 - pvd through exti line detection interrupt */
	dv_irq_tamper,			/* 02 - tamper interrupt */
	dv_irq_rtc,				/* 03 - rtc global interrupt */
	dv_irq_flash,			/* 04 - flash global interrupt */
	dv_irq_rcc,				/* 05 - rcc global interrupt */
	dv_irq_exti0,			/* 06 - exti line0 interrupt */
	dv_irq_exti1,			/* 07 - exti line1 interrupt */
	dv_irq_exti2,			/* 08 - exti line2 interrupt */
	dv_irq_exti3,			/* 09 - exti line3 interrupt */
	dv_irq_exti4,			/* 10 - exti line4 interrupt */
	dv_irq_dma1_channel1,	/* 11 - dma1 channel1 global interrupt */
	dv_irq_dma1_channel2,	/* 12 - dma1 channel2 global interrupt */
	dv_irq_dma1_channel3,	/* 13 - dma1 channel3 global interrupt */
	dv_irq_dma1_channel4,	/* 14 - dma1 channel4 global interrupt */
	dv_irq_dma1_channel5,	/* 15 - dma1 channel5 global interrupt */
	dv_irq_dma1_channel6,	/* 16 - dma1 channel6 global interrupt */
	dv_irq_dma1_channel7,	/* 17 - dma1 channel7 global interrupt */
	dv_irq_adc1_2,			/* 18 - adc1 and adc2 global interrupt */
	dv_irq_usb_hp_can_tx,	/* 19 - usb high priority or can tx interrupts */
	dv_irq_usb_lp_can_rx0,	/* 20 - usb low priority or can rx0 interrupts */
	dv_irq_can_rx1,			/* 21 - can rx1 interrupt */
	dv_irq_can_sce,			/* 22 - can sce interrupt */
	dv_irq_exti9_5,			/* 23 - exti line[9:5] interrupts */
	dv_irq_tim1_brk,		/* 24 - tim1 break interrupt */
	dv_irq_tim1_up,			/* 25 - tim1 update interrupt */
	dv_irq_tim1_trg_com,	/* 26 - tim1 trigger and commutation interrupts */
	dv_irq_tim1_cc,			/* 27 - tim1 capture compare interrupt */
	dv_irq_tim2,			/* 28 - tim2 global interrupt */
	dv_irq_tim3,			/* 29 - tim3 global interrupt */
	dv_irq_tim4,			/* 30 - tim4 global interrupt */
	dv_irq_i2c1_ev,			/* 31 - i2c1 event interrupt */
	dv_irq_i2c1_er,			/* 32 - i2c1 error interrupt */
	dv_irq_i2c2_ev,			/* 33 - i2c2 event interrupt */
	dv_irq_i2c2_er,			/* 34 - i2c2 error interrupt */
	dv_irq_spi1,			/* 35 - spi1 global interrupt */
	dv_irq_spi2,			/* 36 - spi2 global interrupt */
	dv_irq_usart1,			/* 37 - usart1 global interrupt */
	dv_irq_usart2,			/* 38 - usart2 global interrupt */
	dv_irq_usart3,			/* 39 - usart3 global interrupt */
	dv_irq_exti15_10,		/* 40 - exti line[15:10] interrupts */
	dv_irq_rtcalarm,		/* 41 - rtc alarm through exti line interrupt */
	dv_irq_usbwakeup,		/* 42 - usb wakeup from suspend through exti line interrupt */
	dv_irq_tim8_brk,		/* 43 - tim8 break interrupt */
	dv_irq_tim8_up,			/* 44 - tim8 update interrupt */
	dv_irq_tim8_trg_com,	/* 45 - tim8 trigger and commutation interrupts */
	dv_irq_tim8_cc,			/* 46 - tim8 capture compare interrupt */
	dv_irq_adc3,			/* 47 - adc3 global interrupt */
	dv_irq_fsmc,			/* 48 - fsmc global interrupt */
	dv_irq_sdio,			/* 49 - sdio global interrupt */
	dv_irq_tim5,			/* 50 - tim5 global interrupt */
	dv_irq_spi3,			/* 51 - spi3 global interrupt */
	dv_irq_uart4,			/* 52 - uart4 global interrupt */
	dv_irq_uart5,			/* 53 - uart5 global interrupt */
	dv_irq_tim6,			/* 54 - tim6 global interrupt */
	dv_irq_tim7,			/* 55 - tim7 global interrupt */
	dv_irq_dma2_channel1,	/* 56 - dma2 channel1 global interrupt */
	dv_irq_dma2_channel2,	/* 57 - dma2 channel2 global interrupt */
	dv_irq_dma2_channel3,	/* 58 - dma2 channel3 global interrupt */
	dv_irq_dma2_channel4_5,	/* 59 - dma2 channel4 and dma2 channel5 global interrupts */
	dv_nvic_nirq			/* 60 - no. of interrupts (must be last) */
};

typedef enum dv_irqid_e dv_irqid_t;

#endif
