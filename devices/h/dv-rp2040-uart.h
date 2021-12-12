/*  dv-rp2040-uart.h
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
#ifndef DV_RP2040_UART_H
#define DV_RP2040_UART_H

#include <dv-devices.h>

#define DV_RP2040_UART0_BASE		0x40034000
#define DV_RP2040_UART1_BASE		0x40038000

typedef struct dv_rp2040_uart_s
{
	volatile dv_u32_t	dr;			/* 0x000 - data */
	volatile dv_u32_t	rsr;		/* 0x004 - receiver status */
	volatile dv_u32_t	gap1[4];
	volatile dv_u32_t	fr;			/* 0x018 - flag */
	volatile dv_u32_t	gap2[1];
	volatile dv_u32_t	ilpr;		/* 0x020 - IrDA low-power counter */
	volatile dv_u32_t	ibrd;		/* 0x024 - integer baud rate divisor */
	volatile dv_u32_t	fbrd;		/* 0x028 - fractional baud rate divider */
	volatile dv_u32_t	lcr_h;		/* 0x02c - line control*/
	volatile dv_u32_t	cr;			/* 0x030 - control */
	volatile dv_u32_t	ifls;		/* 0x034 - interrupt fifo level select */
	volatile dv_u32_t	imsc;		/* 0x038 - interrupt mask set/clear */
	volatile dv_u32_t	ris;		/* 0x03c - raw interrupt status */
	volatile dv_u32_t	mis;		/* 0x040 - masked interrupt status */
	volatile dv_u32_t	icr;		/* 0x044 - interrupt clear */
	volatile dv_u32_t	dmacr;		/* 0x048 - DMA control*/
	volatile dv_u32_t	gap3[997];
	volatile dv_u32_t	id[4];		/* 0xfe0 - peripheral ID registers 0..3 */
	volatile dv_u32_t	cellid[4];	/* 0xff0 - peripheral cell ID registers 0..3 */
} dv_rp2040_uart_t;

#define dv_rp2040_uart0		(((dv_rp2040_uart_t *)DV_RP2040_UART0_BASE)[0])
#define dv_rp2040_uart0_xor	(((dv_rp2040_uart_t *)(DV_RP2040_UART0_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_uart0_w1s	(((dv_rp2040_uart_t *)(DV_RP2040_UART0_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_uart0_w1c	(((dv_rp2040_uart_t *)(DV_RP2040_UART0_BASE+DV_OFFSET_W1C))[0])

#define dv_rp2040_uart1		(((dv_rp2040_uart_t *)DV_RP2040_UART1_BASE)[0])
#define dv_rp2040_uart1_xor	(((dv_rp2040_uart_t *)(DV_RP2040_UART1_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_uart1_w1s	(((dv_rp2040_uart_t *)(DV_RP2040_UART1_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_uart1_w1c	(((dv_rp2040_uart_t *)(DV_RP2040_UART1_BASE+DV_OFFSET_W1C))[0])

#define DV_UART_DATA		0xff

/* Receiver status bits. Equivalent bits in DR are shifted left by 8
*/
#define DV_UART_OE			0x08		/* Overflow error */
#define DV_UART_BE			0x04		/* Break */
#define DV_UART_PE			0x02		/* Parity error */
#define DV_UART_FE			0x01		/* Frame error */

#define DV_UART_RI			0x100
#define DV_UART_TXFE		0x080		/* Tx fifo empty */
#define DV_UART_RXFF		0x040		/* Rx fifo full */
#define DV_UART_TXFF		0x020		/* Tx fifo full */
#define DV_UART_RXFE		0x010		/* Rx fifo empty */
#define DV_UART_BUSY		0x008
#define DV_UART_DCD			0x004
#define DV_UART_DSR			0x002
#define DV_UART_CTS			0x001

#define DV_UART_SPS			0x80
#define DV_UART_WLEN		0x60		/* (Nbits-5) << 5 */
#define DV_UART_FEN			0x10		/* FIFO enable */
#define DV_UART_STP2		0x08		/* 2 stop bits (transmit) */
#define DV_UART_EPS			0x04		/* Even parity */
#define DV_UART_PEN			0x02		/* Parity enable */
#define DV_UART_BRK			0x01		/* Break (software-controlled) */

#define DV_UART_CTSEN		0x8000		/* CTS flow control enabled */
#define DV_UART_RTSEN		0x4000		/* RTS flow control enabled */
#define DV_UART_OUT2		0x2000		/* Can be used as RI. Inverted */
#define DV_UART_OUT1		0x1000		/* Can be used as DCD. Inverted */
#define DV_UART_RTS			0x0800		/* RTS. Inverted */
#define DV_UART_DTR			0x0400		/* DTR. Inverted */
#define DV_UART_RXE			0x0200		/* Rx enable */
#define DV_UART_TXE			0x0100		/* Tx enable */
#define DV_UART_LBE			0x0080		/* IrDA */
#define DV_UART_SIRLP		0x0004		/* IrDA */
#define DV_UART_SIREN		0x0002		/* IrDA */
#define DV_UART_UARTEN		0x0001		/* UART enable */

/* dv_uart0_isrx() - returns true if there's a character to read.
*/
static inline int dv_rp2040_uart_isrx(dv_rp2040_uart_t *uart)
{
	return ( (uart->fr & DV_UART_RXFE) == 0 );
}

/* dv_uart0_istx() - returns true if there's room to send a character
*/
static inline int dv_rp2040_uart0_istx(dv_rp2040_uart_t *uart)
{
	return ( (uart->fr & DV_UART_TXFF) == 0 );
}

extern int dv_rp2040_uart_getc(dv_rp2040_uart_t *);
extern void dv_rp2040_uart_putc(dv_rp2040_uart_t *, int);
extern int dv_rp2040_uart_init(ddv_rp2040_uart_t *, unsigned, char *);

#endif
