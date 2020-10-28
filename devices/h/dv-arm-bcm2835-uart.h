/*  dv-arm-bcm2835-uart.h - UART on bcm2835 etc. (raspberry pi)
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
#ifndef dv_arm_bcm2835_uart_h
#define dv_arm_bcm2835_uart_h	1

#include <dv-devices.h>

#ifndef DV_PBASE
#error	"No definition of DV_PBASE in the board headers. Please fix!"
#endif

#if !DV_ASM

/* BCM2835 mini uart (as used on Raspberry Pi CPUs).
 *
 * From the Broadcom documentation (BCM2835-ARM-Peripherals.pdf):
 * "The implemented UART is not a 16650 compatible UART However as far as possible the
 *  first 8 control and status registers are laid out like a 16550 UART. All 16550 register bits
 *  which are not supported can be written but will be ignored and read back as 0. All control
 *  bits for simple UART receive/transmit operations are available."
 *
 * The device is represented here using 32-bit words for the registers.
 * Although the first 7 registers are only 8-bits wide, using the ARM as a little-endian
 * processor (and considering that the padding bytes are read as zero) this
 * should produce an efficient representation.
 *
 * The device must be enabled (see bcm2835 AUX header) and its pins set to the
 * correct mode (see bcm2835 GPIO header) before using the device.
 *
 * Setting the DLAB bit in lcr enables alternative functions for ios and iir (baud high and low)
 * However, it's easier to use the baud register. Therefore no define for the DLAB bit
 *
 * The baud register: lower 16 bits contains baud divider
*/
typedef struct dv_arm_bcm2835_uart_s dv_arm_bcm2835_uart_t;

struct dv_arm_bcm2835_uart_s
{
	dv_reg32_t io;			/* 0x00	i/o data (tx/rx)	*/
	dv_reg32_t ier;			/* 0x04	interrupt enable	*/
	dv_reg32_t iir;			/* 0x08	interrupt identify	*/
	dv_reg32_t lcr;			/* 0x0c	line control		*/
	dv_reg32_t mcr;			/* 0x10	modem control		*/
	dv_reg32_t lsr;			/* 0x14	line status			*/
	dv_reg32_t msr;			/* 0x18	modem status		*/
	dv_reg32_t scratch;		/* 0x1c	scratch				*/
	dv_reg32_t cntl;		/* 0x20	extra control		*/
	dv_reg32_t stat;		/* 0x24	extra status		*/
	dv_reg32_t baud;		/* 0x28	baudrate			*/
};

#define dv_arm_bcm2835_uart	((dv_arm_bcm2835_uart_t *)(DV_PBASE+0x215040))[0]

void dv_arm_bcm2835_uart_init(dv_u32_t baud, dv_u32_t bits, dv_u32_t parity);
void dv_arm_bcm2835_uart_console(void);

#endif

#define DV_IER_TxInt	0x02
#define DV_IER_RxInt	0x01

#define DV_IIR_Iid		0x06	/* Apparently both bits set isn't possible */
#define DV_IIR_Iid_Rx	0x04
#define DV_IIR_Iid_Tx	0x02
#define DV_IIR_NotPend	0x01	/* Clear when interrupt pending */

#define DV_LCR_Break	0x40	/* 1 = Tx pulled low. Hold for 12 bit times for a break */
#define DV_LCR_WordLen	0x03	/* Only 7-bit and 8-bit supported. BCM document is incorrect about bit. */
#define DV_LCR_7bit		0x02
#define DV_LCR_8bit		0x03

#define DV_MCR_RTS		0x02

#define DV_LSR_TxIdle	0x40
#define DV_LSR_TxEmpty	0x20	/* Can accept at least one char */
#define DV_LSR_RxOver	0x02	/* Rx overrun (cleared by reading */
#define DV_LSR_RxReady	0x01	/* Rx data available */

#define DV_MSR_CTS		0x20	/* 1 = CTS low */

								/* cntl has various hardware flow control bits in 7..2, not defined here */
#define DV_CNTL_TxEn	0x02	/* 1 = receiver enabled */
#define DV_CNTL_RxEn	0x01	/* 1 = transmitter enabled */

#define DV_STAT_TxNchars	0x0f000000		/* No of chars in Tx fifo (0..8) */
#define DV_STAT_RxNchars	0x000f0000		/* No of chars in Tx fifo (0..8) */
#define DV_STAT_TxDone		0x00000200
#define DV_STAT_TxEmpty		0x00000100
#define DV_STAT_CTS			0x00000080
#define DV_STAT_RTS			0x00000040
#define DV_STAT_TxFull		0x00000020
#define DV_STAT_RxOver		0x00000010
#define DV_STAT_TxIdle		0x00000008
#define DV_STAT_RxIdle		0x00000004
#define DV_STAT_TxSpace		0x00000002
#define DV_STAT_RxChar		0x00000001		/* Receiver fifo contains 1 or more characters */

#endif
