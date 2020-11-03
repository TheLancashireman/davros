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

#define DV_USB_BASE			0x40005C00
#define DV_USB_SRAM_BASE	0x40006000

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

#endif
