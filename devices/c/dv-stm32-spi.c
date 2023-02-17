/*  dv-stm32-spi.c
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
#include <dv-stm32-spi.h>

dv_spi_t * const dv_spi_tbl[2] =
{
	&dv_spi1,
	&dv_spi2
};

/* dv_stm32_spi_init() - initialise the SPI device
*/
int dv_stm32_spi_init(int spi_no, unsigned max_baud, unsigned cr12)
{
	unsigned baud;

	dv_spi_t *spi = dv_stm32_get_spi(spi_no);

	if ( spi == DV_NULL )
		return 1;		/* SPI device not known */

	if ( spi_no == 1 )
		baud = 72000000/2;
	else
		baud = 36000000/2;

	unsigned br_div = 0;

	while ( baud > max_baud )
	{
		baud /= 2;
		br_div++;
	}

	if ( br_div > 7 )
		return 2;		/* Max baud unsupported */

	dv_u16_t cr1 = (cr12 & ~(DV_SPI_BR | DV_SPI_SPE)) | (br_div << DV_SPI_BR_SHIFT);
	dv_u16_t cr2 = cr12 >> 16;

	/* Parameters OK
	 * First, let's kill the SPI device. This sets SPE=0
	 * Delay needed?
	*/
	spi->cr[0] = 0;

	/* Now proram the desired modes.
	 * Note that the cr12 variable has SPE=0
	*/
	spi->cr[0] = cr1;
	spi->cr[1] = cr2;

	/* Finally, enable the device
	*/
	spi->cr[0] = cr1 | DV_SPI_SPE;

	return 0;
}

/* dv_stm32_spi_disable() - disable the SPI device
 *
 * This is a hard disable. If a transaction is in progress, it gets killed.
*/
void dv_stm32_spi_disable(int spi_no)
{
    dv_spi_t *spi = dv_stm32_get_spi(spi_no);

	if ( spi == DV_NULL )
		return;

	spi->cr[0] = 0;
	spi->cr[1] = 0;
}
