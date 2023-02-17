/*  dv-stm32-spi.h
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
#ifndef DV_STM32_SPI_H
#define DV_STM32_SPI_H

#include "dv-devices.h"

typedef struct dv_spi_s dv_spi_t;

struct dv_spi_s
{
	dv_reg32_t cr[2];		/* Control */
	dv_reg32_t sr;			/* Status */
	dv_reg32_t dr;			/* Data */
	dv_reg32_t poly;		/* CRC polynomial */
	dv_reg32_t rxcrc;		/* Rx CRC */
	dv_reg32_t txcrc;		/* Tx CRC */
	dv_reg32_t i2scfg;		/* I2S configuration */
	dv_reg32_t i2sp;		/* I2S prescaler */
};

#define DV_SPI1_BASE		0x40013000
#define DV_SPI2_BASE		0x40003800
#define DV_SPI3_BASE		0x40003C00

#define dv_spi1			((dv_spi_t *)DV_SPI1_BASE)[0]
#define dv_spi2			((dv_spi_t *)DV_SPI2_BASE)[0]
#define dv_spi3			((dv_spi_t *)DV_SPI3_BASE)[0]

/* cr[0]
*/
#define DV_SPI_BIDIMODE		0x8000
#define DV_SPI_BIDIOE		0x4000
#define DV_SPI_CRCEN		0x2000
#define DV_SPI_CRNEXT		0x1000
#define DV_SPI_DFF			0x0800
#define DV_SPI_RXONLY		0x0400
#define DV_SPI_SSM			0x0200
#define DV_SPI_SSI			0x0100
#define DV_SPI_LSBFIRST		0x0080
#define DV_SPI_SPE			0x0040
#define DV_SPI_BR			0x0038	/* 0 = fpclk/2 to 7 = fpclk/256 ... divide by 2**(n+1) */
#define DV_SPI_BR_SHIFT		3
#define DV_SPI_MSTR			0x0004
#define DV_SPI_CPOL			0x0002
#define DV_SPI_CPHA			0x0001

/* cr[1]
*/
#define DV_SPI_TXEIE		0x0080
#define DV_SPI_RXNEIE		0x0040
#define DV_SPI_ERRIE		0x0020
#define DV_SPI_SSOE			0x0004
#define DV_SPI_TXDMAEN		0x0002
#define DV_SPI_RXDMAEN		0x0001

/* sr
*/
#define DV_SPI_BSY			0x0080
#define DV_SPI_OVR			0x0040
#define DV_SPI_MODF			0x0020
#define DV_SPI_CRCERR		0x0010
#define DV_SPI_UDR			0x0008
#define DV_SPI_CHSIDE		0x0004
#define DV_SPI_TXE			0x0002
#define DV_SPI_RXNE			0x0001

/* is2cfg and i2sp : out of scope for this file
*/

/* Table of SPI peripherals.
*/
extern dv_spi_t * const dv_spi_tbl[2];

/* dv_stm32_get_spi() - return address of SPI peripheral
*/
static inline dv_spi_t *dv_stm32_get_spi(int spi_no)
{
	if ( (spi_no < 1) || (spi_no > 2) )
		return DV_NULL;
	return dv_spi_tbl[spi_no];
}

/* dv_stm32_spi_isrx() - returns true if there's a character to read.
*/
static inline int dv_stm32_spi_isrx(dv_spi_t *spi)
{
	return ( (spi->sr & DV_SPI_RXNE) != 0 );
}

/* dv_stm32_spi_waitrx() - wait until there's a character to read.
*/
static inline void dv_stm32_spi_waitrx(dv_spi_t *spi)
{
	while ( !dv_stm32_spi_isrx(spi) )
	{
	}
}

/* dv_stm32_spi_read_dr() - read and return the data register
*/
static inline dv_u16_t dv_stm32_spi_read_dr(dv_spi_t *spi)
{
	return (dv_u16_t)spi->dr;
}

/* dv_stm32_spi_get() - wait for data and return it
*/
static inline dv_u16_t dv_stm32_spi_get(dv_spi_t *spi)
{
	dv_stm32_spi_waitrx(spi);
	return dv_stm32_spi_read_dr(spi);
}

/* dv_stm32_spi_istx() - returns true if there's room to send a character
*/
static inline int dv_stm32_spi_istx(dv_spi_t *spi)
{
	return ( (spi->sr & DV_SPI_TXE) != 0 );
}

/* dv_stm32_spi_waittx() - wait until there's room to send a character
*/
static inline void dv_stm32_spi_waittx(dv_spi_t *spi)
{
	while ( !dv_stm32_spi_istx(spi) )
	{
	}
}

/* dv_stm32_spi_write_dr() - write data to the data register
*/
static inline void dv_stm32_spi_write_dr(dv_spi_t *spi, dv_u16_t d)
{
	spi->dr = d;
}

/* dv_stm32_spi_put() - wait for space in tx then write data
*/
static inline void dv_stm32_spi_put(dv_spi_t *spi, dv_u16_t d)
{
	dv_stm32_spi_waittx(spi);
	dv_stm32_spi_write_dr(spi, d);
}

extern int dv_stm32_spi_init(int spi_no, unsigned max_baud, unsigned mode);
extern void dv_stm32_spi_disable(int spi_no);

#endif
