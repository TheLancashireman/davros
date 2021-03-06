/*  dv-stm32-usb.c
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
#include "dv-stm32-usb.h"
#include "dv-stm32-gpio.h"
#include "dv-stm32-rcc.h"
#include "dv-usb-util.h"
#include "dv-usb-log.h"
#include "dv-usb.h"

#include DV_USB_CALLOUT

#if DV_CFG_USB_N_ENDPOINTS <= 0
#error "USB support disabled. Ensure that DV_CFG_USB_N_ENDPOINTS is defined in your dv-config.h"
#endif

static void dv_stm32_usb_error(void);
static void dv_stm32_usb_esof(void);
static void dv_stm32_usb_overrun(void);
static void dv_stm32_usb_reset(void);
static void dv_stm32_usb_sof(void);
static void dv_stm32_usb_suspend(void);
static void dv_stm32_usb_transfer(dv_u32_t istr);
static void dv_stm32_usb_wakeup(void);

dv_u32_t dv_free_pbuf;

/* dv_stm32_usb_init() - initialise the usb controller
 *
 * See also dv_stm32_usb_connect()
 *
 * Initialisation sequence (from sect. 23.4.2 of STM32F10x reference manual):
 *	- provide clock signals to USB controller (rcc?)
 *	- de-assert reset signal (rcc?)
 *	- turn on analog circuitry (de-assert PDWN)
 *	- wait Tstartup (see data sheet)
 *	- de-assert controller reset (FRES)
 *	- clear spurious interrupts (ISTR)
 *	- initialise the packet buffer table (probably before removing PDWN?)
 *
 * From data sheet:
 *	- Tstartup is Max 1 us !!!  So only have to wait 1us :-)
 *	- HSE aand PLL mustr be enabled, USBCLK = 48 MHz
 *	- USB pins USB_DP and USB_DM are  PA12 and PA11 resp. Automatically configured when USB enabled.
*/
void dv_stm32_usb_init(void)
{
	/* Assert the reset signal
	*/
	dv_rcc.apb1rst |= DV_RCC_USB;

	/* Ensure a 1.5 prescaler to divide the 72 MHz down to 48 MHz for the USB port
	*/
	dv_rcc.cfg &= ~DV_RCC_USBPRE;

	/* Enable the clock to the USB peripheral
	*/
	dv_rcc.apb1en |= DV_RCC_USB;

	/* Deassert the reset signal
	*/
	dv_rcc.apb1rst &= ~DV_RCC_USB;

	/* Ensure that both FRES and PDWN are set
	 * This also clears all the interrupt enable flags
	*/
	dv_usb.cntr = (DV_USB_FRES | DV_USB_PDWN);

	/* Set the SRAM to funny values
	 * Don't forget the differing 16/32-bit views!
	*/
	dv_u32_t *p = (dv_u32_t *)DV_USB_SRAM_BASE;
	for ( int i = 0; i < (DV_USB_SRAM_LENGTH/2); i++ )
		*p++ = 0xdead;
	
	dv_stm32_usb_connect();
}

/* dv_stm32_usb_connect() - initialisation part 2
 *
 * Placed in a separate function to allow disconnect and reconnect
*/
void dv_stm32_usb_connect(void)
{
	/* De-assert PDWN
	*/
	dv_usb.cntr &= ~DV_USB_PDWN;

	/* Allow time for analog circuitry to settle - 1 us according to data sheet
	*/
	for (volatile int delay = 0; delay < DV_USB_TSTARTUP_DELAY; delay++ )
	{	/* Nothing */	}

	/* Clear any spurious or leftover interrupts
	*/
	dv_usb.istr = 0;

	/* De-assert controller reset
	*/
	dv_usb.cntr &= ~DV_USB_FRES;

	/* Clear any spurious or leftover interrupts
	*/
	dv_usb.istr = 0;

	/* Enable the reset interrupt
	*/
	dv_usb.cntr |= DV_USB_RESETM;
}

/* dv_stm32_usb_disconnect() - go to power-down
*/
void dv_stm32_usb_disconnect(void)
{
	/* Clear any spurious or leftover interrupts
	*/
	dv_usb.istr = 0;

	/* Assert controller reset and power-down
	*/
	dv_usb.cntr = DV_USB_FRES | DV_USB_PDWN;

	/* Clear any spurious or leftover interrupts
	*/
	dv_usb.istr = 0;
}

/* dv_stm32_usb_error() - handle an error interrupt
*/
static void dv_stm32_usb_error(void)
{
	/* Nothing to do */
}

/* dv_stm32_usb_esof() - handle an expected start-of-frame interrupt
*/
static void dv_stm32_usb_esof(void)
{
	/* Nothing to do */
}

/* dv_stm32_usb_overrun() - handle an overrun error interrupt
*/
static void dv_stm32_usb_overrun(void)
{
	/* Nothing to do */
}

/* dv_stm32_usb_reset() - handle a reset interrupt
*/
static void dv_stm32_usb_reset(void)
{
	dv_printf("dv_stm32_usb_reset()\n");
	/* Clear all interrupt status
	*/
	dv_usb.istr = 0;	

	/* Enable reset and transfer interrupts, and any others that the application wants.
	*/
	dv_usb.cntr = DV_USB_CTRM | DV_USB_RESETM | DV_CFG_USB_INTERRUPTS;

	/* Give the peripheral the offset of the buffer table
	*/
	dv_usb.btable = DV_BTABLE_OFFSET;

	/* Set up the control endpoint (EP0)
	*/
	dv_free_pbuf = DV_BTABLE_LEN;
	dv_configure_pbuf(0, DV_CFG_EP0_SIZE, DV_CFG_EP0_SIZE);
	dv_usb.epr[0] = DV_USB_CONTROL | DV_USB_RX_VALID;

	/* Set the device address to default (0) and enable the function.
	*/
	dv_usb.daddr = DV_USB_EF | 0;
}

/* dv_stm32_usb_sof() - handle a start-of-frame interrupt
*/
static void dv_stm32_usb_sof(void)
{
	/* Nothing to do */
}

/* dv_stm32_usb_suspend() - handle a suspend interrupt
 *
 * Suspend, then go to low-power mode
*/
static void dv_stm32_usb_suspend(void)
{
	dv_usb.cntr |= DV_USB_FSUSP;
	dv_usb.cntr |= DV_USB_LP_MODE;
}

/* dv_stm32_usb_transfer() - handle a transfer interrupt
 *
 * The parameter contains the endpoint number and the direction flag in the lower bits
 *
 * Don't forget that some bits of the epr register are "write 1 to invert" so we write 0 to those.
 * Also, the CTR_RX and CTR_TX are "write 0 to clear" so we write 1 to the one we don't want to clear.
*/
static void dv_stm32_usb_transfer(dv_u32_t istr)
{
	dv_u16_t ep = istr & DV_USB_EP_ID;
	dv_u16_t epstat = dv_usb.epr[ep];

	dv_printf("dv_stm32_usb_transfer(): ep = %d, epstat = 0x%04x\n", ep, epstat);

	if ( epstat & DV_USB_CTR_RX )
	{
		dv_usb.epr[ep] = (epstat & (DV_USB_SETUP | DV_USB_EP_TYPE | DV_USB_EP_KIND | DV_USB_EA)) | DV_USB_CTR_TX;
		if ( ep < DV_CFG_USB_N_ENDPOINTS )
		{
			if ( (epstat & DV_USB_SETUP) != 0 )
				dv_usb_dev.epstate[ep].setup_func(ep);
			else
				dv_usb_dev.epstate[ep].rx_func(ep);
		}
	}
	if ( epstat & DV_USB_CTR_TX )
	{
		dv_usb.epr[ep] = (epstat & (DV_USB_SETUP | DV_USB_EP_TYPE | DV_USB_EP_KIND | DV_USB_EA )) | DV_USB_CTR_RX;
		if ( ep < DV_CFG_USB_N_ENDPOINTS )
		{
			dv_usb_dev.epstate[ep].tx_func(ep);
		}
	}
}

/* dv_stm32_usb_wakeup() - handle a wakeup interrupt
*/
static void dv_stm32_usb_wakeup(void)
{
	dv_usb.cntr |= ~DV_USB_FSUSP;
}

/* dv_stm32_usb_lp_isr() - service routine for USB low-priority interrupt
*/
void dv_stm32_usb_lp_isr(void)
{
	dv_u32_t istr;
	dv_u32_t events = 0;

	/* Read the interrupt status
	*/
	istr = dv_usb.istr;

	dv_usb_log(0xee, 0x01, (dv_u16_t)istr, 0x0000, 0x0000);

	if ( (istr & DV_USB_RESETM)	!= 0 )
	{
		/* dv_stm32_usb_reset() clears all interrupts and reconfigures the device, so
		 * we can bomb out afterwards.
		*/
		dv_stm32_usb_reset();
		callout_usb_reset();
		return;
	}

	if ( (istr & DV_USB_SUSPM) != 0 )
	{
		dv_stm32_usb_suspend();
		events |= DV_CFG_USB_SUSPEND_EVT;
		dv_usb.istr = ~DV_USB_SUSPM;
	}

	if ( (istr & DV_USB_WKUPRM) != 0 )
	{
		dv_stm32_usb_wakeup();
		events |= DV_CFG_USB_WAKEUP_EVT;
		dv_usb.istr = ~DV_USB_WKUPRM;
	}

	if ( (istr & DV_USB_ERRM) != 0 )
	{
		dv_stm32_usb_error();
		events |= DV_CFG_USB_ERROR_EVT;
		dv_usb.istr = ~DV_USB_ERRM;
	}

	if ( (istr & DV_USB_PMAOVRM) != 0 )
	{
		dv_stm32_usb_overrun();
		events |= DV_CFG_USB_OVERRUN_EVT;
		dv_usb.istr = ~DV_USB_PMAOVRM;
	}

	if ( (istr & DV_USB_SOFM) != 0 )
	{
		dv_stm32_usb_sof();
		events |= DV_CFG_USB_SOF_EVT;
		dv_usb.istr = ~DV_USB_SOFM;
	}

	if ( (istr & DV_USB_ESOFM) != 0 )
	{
		dv_stm32_usb_esof();
		events |= DV_CFG_USB_ESOF_EVT;
		dv_usb.istr = ~DV_USB_ESOFM;
	}

	while ( (istr & DV_USB_CTRM) != 0 )
	{
		dv_usb.istr = ~DV_USB_CTRM;
		dv_stm32_usb_transfer(istr);
		events |= callout_usb_transfer(istr);

		/* Re-read the interrupt status to see if there are more EPs to process
		*/
		istr = dv_usb.istr;
	}

	if ( events != 0 )
		callout_usb_events(events);
}

/* dv_stm32_usb_write_ep() - write some data to an EP
 *
 * Returns the number of bytes copied to the SRAM.
 * The SRAM can only be accessed 16 bits at a time.
*/
dv_i32_t dv_stm32_usb_write_ep(dv_i32_t ep, const dv_u8_t *src, dv_i32_t n)
{
	dv_printf("dv_stm32_usb_write_ep() : ep = %d, n = %d\n", ep, n);

	/* Calculate the address of the EP buffer in the dual-port SRAM
	*/
	dv_u32_t *buf = &((dv_u32_t *)DV_USB_SRAM_BASE)[dv_btable.ep[ep].buf[DV_USB_EPB_TX].addr/2];

	/* Write n/2 words (16 bit) to the SRAM
	*/
	int i, j;
	dv_printf("... data =");
	for ( i=0, j=0; i<(n/2); i++, j+=2 )
	{
		buf[i] = dv_usb_load_16(&src[j]);
		dv_printf(" 0x%04x", buf[i]);
	}

	/* Write the final byte if the length is odd
	*/
	if ( n%2 != 0 )
	{
		buf[i] = src[j];
		dv_printf(" 0x%04x", buf[i]);
	}
	dv_printf("\n");

	/* Write the number of bytes to send
	*/
	dv_btable.ep[ep].buf[DV_USB_EPB_TX].count = n;

	/* Set the STAT_TX bits of the EP register to TX_VALID.
	*/
	dv_stm32_usb_set_ep_stat_tx(ep, DV_USB_TX_VALID);


	return n;
}

/* dv_stm32_usb_read_ep() - read the data from an EP
 *
 * Returns the number of bytes copied to the destination.
 * The SRAM can only be accessed 16 bits at a time.
*/
dv_i32_t dv_stm32_usb_read_ep(dv_i32_t ep, dv_u8_t *dest, dv_i32_t max)
{
	dv_printf("dv_stm32_usb_read_ep(): ep = %d, dv_btable.ep[ep].buf[DV_USB_EPB_RX].addr = 0x%04x, count = 0x%04x\n",
			ep, dv_btable.ep[ep].buf[DV_USB_EPB_RX].addr, dv_btable.ep[ep].buf[DV_USB_EPB_RX].count);

	/* Calculate the address of the EP buffer in the dual-port SRAM
	*/
	dv_u32_t *buf = &((dv_u32_t *)DV_USB_SRAM_BASE)[dv_btable.ep[ep].buf[DV_USB_EPB_RX].addr/2];

	/* Get the no. of bytes received
	*/
	dv_i32_t len = dv_btable.ep[ep].buf[DV_USB_EPB_RX].count & DV_USB_COUNT;

	dv_printf("dv_stm32_usb_read_ep(): ep = %d, buf = 0x%08x\n", ep, (dv_u32_t)buf);
	dv_printf("... buf[0] = 0x%04x, buf[1] = 0x%04x, buf[2] = 0x%04x, buf[3] = 0x%04x, len = %d\n",
				buf[0], buf[1], buf[2], buf[3], len);

	/* Ensure we don't write more than the destination buffer size
	*/
	if ( len > max )
		len = max;

	dv_u32_t i, j;
	dv_u16_t w = 0;		/* Initialisation here mutes a warning "w might be used uninitialised" in the else branch */

	/* Read words from the EP buffer and write bytes to the destination buffer
	*/
	for ( i = 0, j = 0; i < len; i++ )
	{
		if ( (i & 0x01) == 0 )
		{
			w = buf[j++];
			dest[i] = w & 0xff;		/* Little endian ==> LS byte first */
		}
		else
			dest[i] = w >> 8;
	}

	/* Set the STAT_RX bits of the EP register back to RX_VALID. The hardware has set them to something
	 * else (RX_NAK?) on packet reception
	*/
	dv_stm32_usb_set_ep_stat_rx(ep, DV_USB_RX_VALID);

	return len;
}

/* dv_configure_pbuf() - configure a packet buffer
 *
 * Note: isochronous/double-buffered endpoints not supported.
*/
void dv_configure_pbuf(dv_i32_t ep, dv_u32_t tx_size, dv_u32_t rx_size)
{
	if ( ep < DV_CFG_USB_N_ENDPOINTS )
	{
		if ( tx_size <= 0 )
		{
			/* TX not used. */
		}
		else
		{
			dv_u16_t n_blocks = (tx_size+1) / 2;
			dv_btable.ep[ep].buf[DV_USB_EPB_TX].addr = dv_alloc_pbuf(n_blocks * 2);
			dv_btable.ep[ep].buf[DV_USB_EPB_TX].count = 0;
		}

		if ( rx_size <= 0 )
		{
			/* RX not used. */
		}
		else if ( rx_size <= 62 )
		{
			/* Block size = 2 bytes
			*/
			dv_u16_t n_blocks = (rx_size+1) / 2;		/* Round up */
			dv_btable.ep[ep].buf[DV_USB_EPB_RX].addr = dv_alloc_pbuf(n_blocks * 2);
			dv_btable.ep[ep].buf[DV_USB_EPB_RX].count = DV_USB_NBLK_TO_REG(n_blocks);
		}
		else
		{
			/* Block size 32 bytes
			*/
			dv_u16_t n_blocks = (rx_size+31) / 32;		/* Round up */
			dv_btable.ep[ep].buf[DV_USB_EPB_RX].addr = dv_alloc_pbuf(n_blocks * 32);
			dv_btable.ep[ep].buf[DV_USB_EPB_RX].count = DV_USB_NBLK_TO_REG(n_blocks-1) | DV_USB_BL_SIZE;
		}
	}
}
