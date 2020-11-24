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
#include DV_USB_CONFIG

/* Vendor and device IDs that could be "borrowed"
*/
#define DV_USB_VENDOR_ID_STM					0x0483		/* STMicroelectronics vendor ID */
#define DV_USB_DEVICE_ID_STM_INTERRUPT_DEMO		0x5721		/* STM interrupt demo device */
#define DV_USB_DEVICE_ID_STM_BULK_DEMO	 		0x5722		/* STM bulk demo device */

/* DV_CFG_USB_N_ENDPOINTS is a configuration parameter. If it is zero or undefined, USB is not used.
*/
#ifndef DV_CFG_USB_N_ENDPOINTS
#define DV_CFG_USB_N_ENDPOINTS	0
#endif

#if DV_CFG_USB_N_ENDPOINTS > 0

/* dv_stm32usb_t - a structure to model the registers in the USB device.
*/
struct dv_stm32usb_s
{
	dv_reg32_t epr[16];			/* Endpoint registers. "Up to 8" are implemented */
	dv_reg32_t cntr;			/* Control register */
	dv_reg32_t istr;			/* Interrupt status register */
	dv_reg32_t fnr;				/* Frame number register */
	dv_reg32_t daddr;			/* Device address register */
	dv_reg32_t btable;			/* Buffer table address */
};

typedef struct dv_stm32usb_s dv_stm32usb_t;

#define DV_USB_BASE			0x40005C00

#define dv_usb		((dv_stm32usb_t *)DV_USB_BASE)[0]

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
#define DV_USB_ISTRM	0xff00	/* All interrupt status flags */
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
 *
 * IMPORTANT! The view of the packet memory from the USB controller is an array of 256 x 16-bit words.
 * The view from the CPU is an array of 256 x 32-bit words, of which only the lower 16 bits contain
 * any information.
 * 
*/
#define DV_USB_SRAM_BASE	0x40006000
#define DV_USB_SRAM_LENGTH	512

typedef struct dv_epbuf_descr_s
{
	dv_u32_t addr;
	volatile dv_u32_t count;
} dv_epbuf_descr_t;

typedef struct dv_ep_descr_s
{
	dv_epbuf_descr_t buf[2];
} dv_ep_descr_t;

#define DV_USB_EPB_TX	0	/* Index of the tx descriptor */
#define DV_USB_EPB_RX	1	/* Index of the rx descriptor */

/* epbuf_descr.count
 *
 * BL_SIZE and N_BLOCK are only used for the RX descriptor.
 * In the RX descriptor, the COUNT field is set by the hardware to indicate how many bytes were received.
 * In the TX descriptor, the COUNT field is set by the software to indicate how many bytes to transmit.
 *
 * This implies that:
 *	- RX_SIZE must be a multiple of 2
 *	- If RX_SIZE is 64 or more, it must be a multiple of 32
*/
#define DV_USB_BL_SIZE	0x8000	/* 0 => block size of 2 bytes; 1 => block size of 32 bytes */
#define DV_USB_N_BLOCK	0x7c00	/* No. of blocks */
#define DV_USB_COUNT	0x03ff	/* No. of bytes received or to transmit */

#define DV_USB_NBLK_TO_REG(x)	((x)<<10)

typedef struct dv_epbuffers_s
{
	dv_ep_descr_t ep[DV_CFG_USB_N_ENDPOINTS];
} dv_epbuffers_t;

/* The buffer table is placed at the end of the buffer pool RAM, so that the actual buffers
 * can be allocated starting from 0
*/
#define DV_BTABLE_OFFSET	0
#define DV_BTABLE_LEN		(DV_CFG_USB_N_ENDPOINTS*8)
#define dv_btable			((dv_epbuffers_t *)(DV_USB_SRAM_BASE+DV_BTABLE_OFFSET))[0]

/* The startup delay. 1 us according to the data sheet. The sample program doesn't delay,
 * but 1 us at 72 MHz must be about 20 times round even the shortest loop.
*/
#define DV_USB_TSTARTUP_DELAY	20

/* Variables
*/
extern dv_u32_t dv_free_pbuf;

/* Functions
*/
extern void dv_stm32_usb_init(void);
extern void dv_stm32_usb_connect(void);
extern void dv_stm32_usb_disconnect(void);
extern void dv_stm32_usb_lp_isr(void);
extern void dv_stm32_usb_hp_isr(void);		/* Not supported */
extern void dv_stm32_usb_wakeup_isr(void);	/* Not supported */
extern void dv_configure_pbuf(dv_i32_t ep, dv_u32_t tx_size, dv_u32_t rx_size);
extern dv_i32_t dv_stm32_usb_read_ep(dv_i32_t, dv_u8_t *, dv_i32_t);
extern dv_i32_t dv_stm32_usb_write_ep(dv_i32_t, const dv_u8_t *, dv_i32_t);

/* dv_alloc_pbuf() - allocate some bytes of packet buffer
*/
static inline dv_u32_t dv_alloc_pbuf(dv_u32_t len)
{
#if 0
	dv_printf("dv_alloc_pbuf() : dv_free_pbuf (before) = 0x%04x, len = %d\n", dv_free_pbuf, len);
#endif
	/* Is there enough free space? Return out-of-range if not.
	*/
	if ( (dv_free_pbuf + len) > DV_USB_SRAM_LENGTH )
		return DV_USB_SRAM_LENGTH;

	/* Allocate the buffer; move the index to the next free byte
	*/
	dv_u32_t alloc = dv_free_pbuf;
	dv_free_pbuf += len;

#if 0
	dv_printf("dv_alloc_pbuf() : dv_free_pbuf (after) = 0x%04x, alloc = 0x%04x\n", dv_free_pbuf, alloc);
#endif
	return alloc;
}

/* dv_stm32_usb_set_ep_stat_tx() - set the endpoint STAT_TX field
 *
 * Sets the STAT_TX field of the endpoint register without changing any other bits.
 *
 * Basically a read/modify/write operation, except:
 *	- CTR_RX and CTR_TX are "write 0 to clear" so must be written with 1
 *	- DTOG_RX, STAT_RX and DTOG_TX are "write 1 to toggle" so must be written with 0
 *	- STAT_TX bits are "write 1 to toggle" so they are XORed with the parameter first
 *	- writing back the STAT_TX bits is an XOR operation, so the whole is STAT_TX = STAT_TX xor PARAM xor STAT_TX
 *    which is equivalent to STAT_TX = PARAM
*/
static inline void dv_stm32_usb_set_ep_stat_tx(dv_i32_t ep, dv_u16_t val)
{
	dv_u32_t reg;
	reg = dv_usb.epr[ep];
	reg ^= val & DV_USB_STAT_TX;
	reg &= DV_USB_SETUP | DV_USB_EP_TYPE | DV_USB_EP_KIND | DV_USB_EA | DV_USB_STAT_TX;
	reg |= DV_USB_CTR_RX | DV_USB_CTR_TX;
	dv_usb.epr[ep] = reg;
}

/* dv_stm32_usb_set_ep_stat_rx() - set the endpoint STAT_RX field
 *
 * Sets the STAT_RX field of the endpoint register without changing any other bits.
 *
 * Basically a read/modify/write operation, except:
 *	- CTR_RX and CTR_TX are "write 0 to clear" so must be written with 1
 *	- DTOG_RX, STAT_TX and DTOG_TX are "write 1 to toggle" so must be written with 0
 *	- STAT_RX bits are "write 1 to toggle" so they are XORed with the parameter first
 *	- writing back the STAT_RX bits is an XOR operation, so the whole is STAT_RX = STAT_RX xor PARAM xor STAT_RX
 *    which is equivalent to STAT_RX = PARAM
*/
static inline void dv_stm32_usb_set_ep_stat_rx(dv_i32_t ep, dv_u16_t val)
{
	dv_u32_t reg;
	reg = dv_usb.epr[ep];
	reg ^= val & DV_USB_STAT_RX;
	reg &= DV_USB_SETUP | DV_USB_EP_TYPE | DV_USB_EP_KIND | DV_USB_EA | DV_USB_STAT_RX;
	reg |= DV_USB_CTR_RX | DV_USB_CTR_TX;
	dv_usb.epr[ep] = reg;
}

#if DV_DAVROSKA
/* dv_stm32_usb_attach_lp_isr() - attach the low-priority interrupt handler.
*/
#include <davroska.h>
#include <dv-stm32-interrupts.h>

static inline dv_id_t dv_stm32_usb_attach_lp_isr(dv_prio_t p)
{
	/* Attach the ISR to the dv_irq_usb_lp_can_rx0 vector
	*/
	return dv_addisr("Usb-lp", &dv_stm32_usb_lp_isr, dv_irq_usb_lp_can_rx0, p);
}
#endif

/* Driver functions required by the USB device stack
*/
static inline dv_i32_t dv_usbdrv_read_ep(dv_i32_t ep, dv_u8_t *dest, dv_i32_t max)
{
	return dv_stm32_usb_read_ep(ep, dest, max);
}

static inline dv_i32_t dv_usbdrv_write_ep(dv_i32_t ep, const dv_u8_t *src, dv_i32_t n)
{
	return  dv_stm32_usb_write_ep(ep, src, n);
}

static inline void dv_usbdrv_stall_rx(dv_i32_t ep)
{
	dv_stm32_usb_set_ep_stat_rx(ep, DV_USB_RX_STALL);
}

static inline void dv_usbdrv_stall_tx(dv_i32_t ep)
{
	dv_stm32_usb_set_ep_stat_tx(ep, DV_USB_TX_STALL);
}

static inline void dv_usbdrv_set_ep_direction(dv_u8_t unused_dir)
{
	/* Nothing to do */
}

static inline void dv_usbdrv_enable_wakeup(void)
{
	/* Nothing to do */
}

static inline void dv_usbdrv_disable_wakeup(void)
{
	/* Nothing to do */
}

#endif
#endif
