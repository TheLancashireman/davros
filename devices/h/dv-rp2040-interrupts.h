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
	dv_irq_timer0,			/* 00 - TIMER_IRQ_0 */
	dv_irq_timer1,			/* 01 - TIMER_IRQ_1 */
	dv_irq_timer2,			/* 02 - TIMER_IRQ_2 */
	dv_irq_timer3,			/* 03 - TIMER_IRQ_3 */
	dv_irq_pwm_wrap,		/* 04 - PWM_IRQ_WRAP */
	dv_irq_usbctrl,			/* 05 - USBCTRL_IRQ */
	dv_irq_xip,				/* 06 - XIP_IRQ */
	dv_irq_pio0_0,			/* 07 - PIO0_IRQ_0 */
	dv_irq_pio0_1,			/* 08 - PIO0_IRQ_1 */
	dv_irq_pio1_0,			/* 09 - PIO1_IRQ_0 */
	dv_irq_pio1_1,			/* 10 - PIO1_IRQ_1 */
	dv_irq_dma_0,			/* 11 - DMA_IRQ_0 */
	dv_irq_dma_1,			/* 12 - DMA_IRQ_1 */
	dv_irq_io_bank0,		/* 13 - IO_IRQ_BANK0 */
	dv_irq_io_qspi,			/* 14 - IO_IRQ_QSPI */
	dv_irq_sio_proc0,		/* 15 - SIO_IRQ_PROC0 */
	dv_irq_sio_proc1,		/* 16 - SIO_IRQ_PROC1 */
	dv_irq_clocks,			/* 17 - CLOCKS_IRQ */
	dv_irq_spi0,			/* 18 - SPI0_IRQ */
	dv_irq_spi1,			/* 19 - SPI1_IRQ */
	dv_irq_uart0,			/* 20 - UART0_IRQ */
	dv_irq_uart1,			/* 21 - UART1_IRQ */
	dv_irq_adc_fifo,		/* 22 - ADC_IRQ_FIFO */
	dv_irq_i2c0,			/* 23 - I2C0_IRQ */
	dv_irq_i2c1,			/* 24 - I2C1_IRQ */
	dv_irq_rtc,				/* 25 - RTC_IRQ */
	dv_irq_26,				/* 26 - No hardware; can be software-triggered */
	dv_irq_27,				/* 27 - No hardware; can be software-triggered */
	dv_irq_28,				/* 28 - No hardware; can be software-triggered */
	dv_irq_29,				/* 29 - No hardware; can be software-triggered */
	dv_irq_30,				/* 30 - No hardware; can be software-triggered */
	dv_irq_31,				/* 31 - No hardware; can be software-triggered */
	dv_nvic_nirq			/* 32 - no. of interrupts (must be last) */
};

typedef enum dv_irqid_e dv_irqid_t;

#endif
