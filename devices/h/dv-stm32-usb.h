/*  dv-stm32-usb.h
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
#ifndef DV_STM32_USB_H
#define DV_STM32_USB_H

#include "dv-devices.h"

/* DV_USB_N_ENDPOINTS is a configuration parameter. If it is zero or undefined, USB is not used.
*/
#ifndef DV_USB_N_ENDPOINTS
#define DV_USB_N_ENDPOINTS	0
#endif

#if DV_USB_N_ENDPOINTS > 0

/* dv_stm32usb_t - a structure to model the registers in the USB device.
*/

#define DV_USB_BASE			0x40005C00

typedef struct dv_stm32usb_s dv_stm32usb_t;

struct dv_stm32usb_s
{
	dv_reg32_t epr[16];			/* Endpoint registers. "Up to 8" are implemented */
	dv_reg32_t cntr;			/* Control register */
	dv_reg32_t istr;			/* Interrupt status register */
	dv_reg32_t fnr;				/* Frame number register */
	dv_reg32_t daddr;			/* Device address register */
	dv_reg32_t btable;			/* Buffer table address */
};

#define dv_usb	((dv_stm32usb_t *)DV_USB_BASE)[0]

/* CNTR
*/
#define DV_USB_CTRM		0x8000	/* Correct transfer interrupt mask */
#define DV_USB_PMAOVRM	0x4000	/* Packet memory area over-/underrun interrupt mask */
#define DV_USB_ERRM		0x2000	/* Error interrupt mask */
#define DV_USB_WKUPRM	0x1000	/* Wakeup interrupt mask */
#define DV_USB_SUSPM	0x0800	/* Suspend mode interrupt mask */
#define DV_USB_RESETM	0x0400	/* USB reset interrupt mask */
#define DV_USB_SOFM		0x0200	/* Start of frame interrupt mask */
#define DV_USB_ESOFM	0x0100	/* Expected start of frame interrupt mask */
#define DV_USB_RESUME	0x0010	/* Resume request */
#define DV_USB_FSUSP	0x0008	/* Force suspend */
#define DV_USB_LP_MODE	0x0004	/* Low power mode */
#define DV_USB_PDWN		0x0002	/* Power down */
#define DV_USB_FRES		0x0001	/* Force USB reset */

/* ISTR
 * Bits 15..8 are the interrupt status bits that correspond with the interrupt enable bits in CNTR[15..8]
*/
#define DV_USB_CTR		0x8000	/* Correct transfer */
#define DV_USB_PMAOVR	0x4000	/* Packet memory area over-/underrun */
#define DV_USB_ERR		0x2000	/* Error */
#define DV_USB_WKUP		0x1000	/* Wakeup */
#define DV_USB_SUSP		0x0800	/* Suspend mode */
#define DV_USB_RESET	0x0400	/* USB reset */
#define DV_USB_SOF		0x0200	/* Start of frame */
#define DV_USB_ESOF		0x0100	/* Expected start of frame */
#define DV_USB_DIR		0x0010	/* Direction of transaction */
#define DV_USB_EP_ID	0x000f	/* Endpoint identifier */

/* FNR
*/
#define DV_USB_RXDP		0x8000	/* Receive data+ line status */
#define DV_USB_RXDM		0x4000	/* Receive data- line status */
#define DV_USB_LCK		0x2000	/* Locked */
#define DV_USB_LSOF		0x1800	/* Lost start-of-frame (counter) */
#define DV_USB_FN		0x07ff	/* Frame number */

/* DADDR
 * EF	- set by software to enable the device.
 * ADD	- contain the USB function address assigned by the host controller.
*/
#define DV_USB_EF		0x0080	/* Enable function */
#define DV_USB_ADD		0x007f	/* Device address */

/* BTABLE
 * Contains the 8-byte-aligned offset of the buffer allocation table within the packet memory.
*/
#define DV_USB_BTABLE	0xfff8	/* Buffer table address */

/* EPR[n] (Endpoint register)
*/
#define DV_USB_CTR_RX	0x8000	/* Correct transfer for reception - write 0 to clear */
#define DV_USB_DTOG_RX	0x4000	/* Data toggle for reception - write 1 to invert */
#define DV_USB_STAT_RX	0x3000	/* Status bits for reception - write 1 to invert */
#define DV_USB_SETUP	0x0800	/* Setup transaction completed */
#define DV_USB_EP_TYPE	0x0600	/* Endpoint type */
#define DV_USB_EP_KIND	0x0100	/* Endpoint kind */
#define DV_USB_CTR_TX	0x0080	/* Correct transfer for transmission - write 0 to clear */
#define DV_USB_DTOG_TX	0x0040	/* Data toggle for transmission - write 1 to invert */
#define DV_USB_STAT_TX	0x0030	/* Status bits for transmission - write 1 to invert */
#define DV_USB_EA		0x000f	/* Endpoint address */

/* Values of DV_USB_STAT_RX
*/
#define DV_USB_RX_DISABLED	0x0000
#define DV_USB_RX_STALL		0x1000
#define DV_USB_RX_NAK		0x2000
#define DV_USB_RX_VALID		0x3000

/* Values of DV_USB_STAT_TX
*/
#define DV_USB_TX_DISABLED	0x0000
#define DV_USB_TX_STALL		0x0010
#define DV_USB_TX_NAK		0x0020
#define DV_USB_TX_VALID		0x0030

/* Values of DV_USB_EP_TYPE
*/
#define DV_USB_BULK			0x0000	/* DV_USB_EP_KIND==1 --> double-buffered */
#define DV_USB_CONTROL		0x0200	/* DV_USB_EP_KIND==1 --> status-out */
#define DV_USB_ISO			0x0400	/* DV_USB_EP_KIND not used */
#define DV_USB_INTERRUPT	0x0600	/* DV_USB_EP_KIND not used */
#define DV_USB_DBL_BUF		DV_USB_EP_KIND	/* only when EP_TYPE==BULK */
#define DV_USB_STATUS_OUT	DV_USB_EP_KIND	/* only when EP_TYPE==CONTROL */

/* Structures for the buffer pool
 *
 * There's a pool of dual-port memory that's used for buffering USB communications.
 * The pool starts at address 0x40006000 (on the STM32F103xx) and is 512 bytes long.
 * Part of the pool is allocated for the buffer table, which is an array of endpoint descriptors.
 * Each endpoint descriptor contains two buffer descriptors.
 * For double-buffered endpoints both are used for either tx or rx, hence double-buffered
 * endpoints are unidirectional.
 * For single-buffered endpoints the first buffer descriptor is for tx and the second is for rx. 
*/
#define DV_USB_SRAM_BASE	0x40006000
#define DV_USB_SRAM_LENGTH	512

typedef struct dv_epbuf_descr_s
{
	dv_u16_t addr;
	dv_u16_t count;
} dv_epbuf_descr_t;

typedef struct dv_ep_descr_s
{
	dv_epbuf_descr_t buf[2];
} dv_ep_descr_t;

#define DV_USB_EPB_TX	0
#define DV_USB_EPB_RX	0

typedef struct dv_epbuffers_s
{
	dv_ep_descr_t ep[DV_USB_N_ENDPOINTS];
} dv_epbuffers_t;

/* The buffer table is placed at the end of the buffer pool RAM, so that the actual buffers
 * can be allocated starting from 0
*/
#define DV_BTABLE_OFFSET	(DV_USB_SRAM_LENGTH-DV_USB_N_ENDPOINTS*8)
#define dv_btable			((dv_epbuffers_t *)(DV_USB_SRAM_BASE+DV_BTABLE_OFFSET))

#endif
#endif
