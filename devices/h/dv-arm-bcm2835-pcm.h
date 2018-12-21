/*	dv-arm-bcm2835-pcm.h - header file for the raspberry pi PCM/I2S interface
 *
 *	Copyright 2018 David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_arm_bcm2835_pcm_h
#define dv_arm_bcm2835_pcm_h	1

#include <dv-devices.h>

typedef struct dv_arm_bcm2835_pcm_s dv_arm_bcm2835_pcm_t;

struct dv_arm_bcm2835_pcm_s
{
	dv_reg32_t	pcm_cs;			/* 0x00 - Control/status */
	dv_reg32s_t	pcm_fifo;		/* 0x04 - FIFO data */
	dv_reg32_t	pcm_mode;		/* 0x08 - Mode */
	dv_reg32_t	pcm_rxc;		/* 0x0c - Receive configuration */
	dv_reg32_t	pcm_txc;		/* 0x10 - Transmit configuration */
	dv_reg32_t	pcm_dreq;		/* 0x14 - DMA request level */
	dv_reg32_t	pcm_inten;		/* 0x18 - Interrupt enables */
	dv_reg32_t	pcm_instc;		/* 0x1c - Interrupt status and clear */
	dv_reg32_t	pcm_gray;		/* 0x20 - Gray mode control */
};

#define dv_bcm2835_pcm		((dv_arm_bcm2835_pcm_t *)(DV_PBASE+0x203000))[0]

/* Bits in pcm_cs
*/
#define DV_PCM_CS_STBY			0x02000000
#define DV_PCM_CS_SYNC			0x01000000
#define DV_PCM_CS_RXSEX			0x00800000
#define DV_PCM_CS_RXF			0x00400000
#define DV_PCM_CS_TXE			0x00200000
#define DV_PCM_CS_RXD			0x00100000
#define DV_PCM_CS_TXD			0x00080000
#define DV_PCM_CS_RXR			0x00040000
#define DV_PCM_CS_TXW			0x00020000
#define DV_PCM_CS_RXERR			0x00010000
#define DV_PCM_CS_TXERR			0x00008000
#define DV_PCM_CS_RXSYNC		0x00004000
#define DV_PCM_CS_TXSYNC		0x00002000
#define DV_PCM_CS_DMAEN			0x00000200
#define DV_PCM_CS_RXTHR			0x00000180
#define DV_PCM_CS_TXTHR			0x00000060
#define DV_PCM_CS_RXCLR			0x00000010
#define DV_PCM_CS_TXCLR			0x00000008
#define DV_PCM_CS_TXON			0x00000004
#define DV_PCM_CS_RXON			0x00000002
#define DV_PCM_CS_EN			0x00000001

/* Bits in pcm_mode
*/
#define DV_PCM_MODE_CLK_DIS		0x10000000
#define DV_PCM_MODE_PDMIN		0x08000000
#define DV_PCM_MODE_PDME		0x04000000
#define DV_PCM_MODE_FRXP		0x02000000
#define DV_PCM_MODE_FTXP		0x01000000
#define DV_PCM_MODE_CLKM		0x00800000
#define DV_PCM_MODE_CLKI		0x00400000
#define DV_PCM_MODE_FSM			0x00200000
#define DV_PCM_MODE_FSI			0x00100000
#define DV_PCM_MODE_FLEN		0x000ffc00
#define DV_PCM_MODE_FSLEN		0x000003ff

/* Bits in pcm_rxc and pcm_txc
*/
#define DV_PCM_xXC_CH1WEX		0x80000000
#define DV_PCM_xXC_CH1EN		0x40000000
#define DV_PCM_xXC_CH1POS		0x3ff00000
#define DV_PCM_xXC_CH1WID		0x000f0000
#define DV_PCM_xXC_CH2WEX		0x00008000
#define DV_PCM_xXC_CH2EN		0x00004000
#define DV_PCM_xXC_CH2POS		0x00003ff0
#define DV_PCM_xXC_CH2WID		0x0000000f

/* Bits in pcm_dreq
*/
#define DV_PCM_DREQ_TX_PANIC	0x7f000000
#define DV_PCM_DREQ_RX_PANIC	0x007f0000
#define DV_PCM_DREQ_TX			0x00007f00
#define DV_PCM_DREQ_RX			0x0000007f

/* Bits in pcm_inten and pcm_instc
*/
#define DV_PCM_INT_RXERR		0x00000008
#define DV_PCM_INT_TXERR		0x00000004
#define DV_PCM_INT_RXR			0x00000002
#define DV_PCM_INT_TXW			0x00000001

/* Bits in pcm_gray
*/
#define DV_PCM_GRAY_RXFIFOLVL	0x003f0000
#define DV_PCM_GRAY_FLUSHED		0x0000fc00
#define DV_PCM_GRAY_RXLEVEL		0x0000f3f0
#define DV_PCM_GRAY_FLUSH		0x00000004
#define DV_PCM_GRAY_CLR			0x00000002
#define DV_PCM_GRAY_EN			0x00000001


/* Write a sample to the output FIFO
*/
static inline int dv_pcm_write(dv_i32_t w)
{
	while ( (dv_bcm2835_pcm.pcm_cs & DV_PCM_CS_TXD) == 0 )
	{
		/* ToDo: check for errors; return error code
		*/
	}
	dv_bcm2835_pcm.pcm_fifo = w;
	return 0;
}

/* Read a sample from the input FIFO
*/
static inline int dv_pcm_read(dv_i32_t *w)
{
	while ( (dv_bcm2835_pcm.pcm_cs & DV_PCM_CS_RXD) == 0 )
	{
		/* ToDo: check for errors; return error code
		*/
	}
	*w = dv_bcm2835_pcm.pcm_fifo;
	return 0;
}

/* Initialise the PCM device for i2s
*/
void dv_pcm_init_i2s(void);

#endif
