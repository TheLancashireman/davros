/*  dv-stm32-uart.h
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
#ifndef DV_STM32_UART_H
#define DV_STM32_UART_H

#include "dv-devices.h"

/* ===== UART ===== */

typedef struct dv_uart_s dv_uart_t;

struct dv_uart_s
{
	dv_reg32_t sr;			/* Status */
	dv_reg32_t dr;			/* Data */
	dv_reg32_t brr;			/* Baud rate */
	dv_reg32_t cr[3];		/* Control */
	dv_reg32_t gtpr;		/* Guard time and prescaler */
};

#define DV_UART1_BASE		0x40013800
#define DV_UART2_BASE		0x40004400
#define DV_UART3_BASE		0x40004800
#define DV_UART4_BASE		0x40004C00
#define DV_UART5_BASE		0x40005000

#define dv_uart1			((dv_uart_t *)DV_UART1_BASE)[0]
#define dv_uart2			((dv_uart_t *)DV_UART2_BASE)[0]
#define dv_uart3			((dv_uart_t *)DV_UART3_BASE)[0]
#define dv_uart4			((dv_uart_t *)DV_UART4_BASE)[0]
#define dv_uart5			((dv_uart_t *)DV_UART5_BASE)[0]

/* sr
*/
#define DV_UART_CTS			0x200			/* CTS has changed. Write 0 to clear. */
#define DV_UART_LBD			0x100			/* LIN break detected. Write 0 to clear. */
#define DV_UART_TXE			0x080			/* Transmit data register empty (can write next byte) */
#define DV_UART_TC			0x040			/* Transmission complete */
#define DV_UART_RXNE		0x020			/* Receive data not empty (data available). */
#define DV_UART_IDLE		0x010			/* Counterpart of TC? What's the timeout? */
#define DV_UART_ORE			0x008			/* Overrun error - data lost */
#define DV_UART_NE			0x004			/* Noise error */
#define DV_UART_FE			0x002			/* Framing error */
#define DV_UART_PE			0x001			/* Parity error */

/* brr
*/
#define DV_UART_MANTISSA	0xfff0			/* Whole number part of baud rate divider */
#define DV_UART_FRACTION	0x000f			/* Fractional part of baud rate divider */

/* cr[0] (CR1)
*/
#define DV_UART_UE			0x2000			/* UART enable */
#define DV_UART_M			0x1000			/* Word length; 0 = 8 data, 1 = 9 data */
#define DV_UART_WAKE		0x0800			/* Wakeup method; 0 = idle, 1 = address mark */
#define DV_UART_PCE			0x0400			/* Parity enable; parity bit is MSB (see "M") */
#define DV_UART_PS			0x0200			/* Parity select; 0 = even, 1 = odd */
#define DV_UART_PEIE		0x0100			/* Enable parity error interrupt */
#define DV_UART_TXEIE		0x0080			/* Enable tx empty interrupt */
#define DV_UART_TCIE		0x0040			/* Enable tx complete interrupt */
#define DV_UART_RXNEIE		0x0020			/* Enable rx not empty interrupt */
#define DV_UART_IDLEIE		0x0010			/* Enable idle interrupt */
#define DV_UART_TE			0x0008			/* Transmit enable */
#define DV_UART_RE			0x0004			/* Receive enable */
#define DV_UART_RWU			0x0002			/* Receiver wakeup */
#define DV_UART_SBK			0x0001			/* Send break */

/* cr[1] (CR2)
*/
#define DV_UART_LINEN		0x4000			/* LIN mode enable */
#define DV_UART_STOP		0x3000			/* Stop bits (mask) */
#define DV_UART_STOP_1		0x0000			/* 1 stop bit */
#define DV_UART_STOP_0_5	0x1000			/* 0.5 stop bit (not on uart4/5) */
#define DV_UART_STOP_2		0x2000			/* 2 stop bits */
#define DV_UART_STOP_1_5	0x3000			/* 1.5 stop bits (not on uart4/5) */
#define DV_UART_CLKEN		0x0800			/* CK pin enabled (not on uart4/5) */
#define DV_UART_CPOL		0x0400			/* CK pin polarity (not on uart4/5) */
#define DV_UART_CPHA		0x0200			/* CK pin phase (not on uart4/5) */
#define DV_UART_LBCL		0x0100			/* Last bit clock pulse (not on uart4/5) */
#define DV_UART_LBDIE		0x0040			/* LIN break interrupt enable */
#define DV_UART_LBDL		0x0020			/* LIN break length; 0 --> 10 bits, 1 --> 11 bits */
#define DV_UART_ADD			0x000f			/* UART node address */

/* cr[2] (CR3)
*/
#define DV_UART_CTSIE		0x0400			/* CTS interrupt enable */
#define DV_UART_CTSE		0x0200			/* CTS enabled (h/w flow control) */
#define DV_UART_RTSE		0x0100			/* RTS enabled (h/w flow control) */
#define DV_UART_DMAT		0x0080			/* DMA enabled for tx (not uart5) */
#define DV_UART_DMAR		0x0040			/* DMA enabled for rx (not uart5) */
#define DV_UART_SCEN		0x0020			/* Smartcard mode enable (not on uart4/5) */
#define DV_UART_NACK		0x0010			/* Smartcard NACK enable (not on uart4/5) */
#define DV_UART_HDSEL		0x0008			/* Half-duplex */
#define DV_UART_IRLP		0x0004			/* IrDA low power */
#define DV_UART_IREN		0x0002			/* IrDA mode enable */
#define DV_UART_EIE			0x0001			/* Error interrupt enable */

/* gtpr
*/
#define DV_UART_GT			0xff00			/* Guard time */
#define DV_UART_PSC			0x00ff			/* Prescaler for LPIrDA/SmartCard */

/* dv_uart_isrx() - returns true if there's a character to read.
*/
static inline int dv_uart1_isrx(dv_uart_t *uart)
{
	return ( (uart->sr & DV_UART_RXNE) != 0 );
}

/* dv_uart1_istx() - returns true if there's room to send a character
*/
static inline int dv_uart_istx(dv_uart_t *uart)
{
	return ( (uart->sr & DV_UART_TXE) != 0 );
}

extern int dv_uart_getc(dv_uart_t *uart);
extern void dv_uart_putc(dv_uart_t *uart, int);
extern int dv_uart_init(dv_uart_t *uart, unsigned baud, char *format);

#endif
