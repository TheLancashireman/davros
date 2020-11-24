/*  dv-stm32-rcc.c
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
#include "dv-stm32-rcc.h"

/* dv_rcc_init() - initialise the PLL to give the fastest CPU clock possible
 *
 * This is currently very specific to the STM32F103C8/BluePill
 * Crystal 8 MHz, CPU speed 72 MHz.
 *
 * It's slightly worrying that the PLL cannot multiply a 4 MHz clock up to 144 MHz so that
 * the input and output can be divided by 2 to ensure nice square waves. But it seems to work.
*/

void dv_rcc_init(void)
{
#if 1
	/* Set up the PLL config
	*/
	dv_rcc.cfg = DV_RCC_PLLSRC		/* PLLSRC = 1 --> HSE */
				 | (0x7<<18)		/* PLLMUL = 7 --> Multiply by 9 */
				 | 0x1				/* SW = 1     --> System clock = HSE */
				 | (0x4<<8);		/* PPRE1 = 4  --> APB1 = SYSCLK/2 */

	dv_rcc.cc = DV_RCC_HSION		/* HSI on */
				| DV_RCC_HSEON		/* HSE ON */
				| (0x10<<3)			/* HSITRIM = 16 --> 8 MHz +/- 1% */
				| DV_RCC_PLLON;

	/* Wait for lock
	*/
	do {
		asm("nop");
	} while ( (dv_rcc.cc & DV_RCC_PLLRDY ) == 0 );

	/* 1 wait state on flash; enable prefetch
	*/
	dv_flash.ac = DV_FLASH_PREFETCH | 0x1;		/* 1 wait states, prefetch */

	/* Switch to PLL
	*/
	dv_rcc.cfg = DV_RCC_PLLSRC		/* PLLSRC = 1 --> HSE */
				 | (0x7<<18)		/* PLLMUL = 7 --> Multiply by 9 */
				 | 0x2				/* SW = 2     --> System clock = PLL */
				 | (0x4<<8);		/* PPRE1 = 4  --> APB1 = SYSCLK/2 */
#else

#define __RCC_CFGR_VAL	0x001d8402
#define __RCC_CR_VAL	0x01010082

  dv_rcc.cfg = __RCC_CFGR_VAL;                        // set clock configuration register
  dv_rcc.cc   = __RCC_CR_VAL;                          // set clock control register

  if (__RCC_CR_VAL & DV_RCC_HSION) {                 // if HSI enabled
    while ((dv_rcc.cc & DV_RCC_HSIRDY) == 0);          // Wait for HSIRDY = 1 (HSI is ready)
  }

  if (__RCC_CR_VAL & DV_RCC_HSEON) {                 // if HSE enabled
    while ((dv_rcc.cc & DV_RCC_HSERDY) == 0);          // Wait for HSERDY = 1 (HSE is ready)
  }

  if (__RCC_CR_VAL & DV_RCC_PLLON) {                 // if PLL enabled
    while ((dv_rcc.cc & DV_RCC_PLLRDY) == 0);          // Wait for PLLRDY = 1 (PLL is ready)
  }

  /* Wait till SYSCLK is stabilized (depending on selected clock) */
  while ((dv_rcc.cfg & DV_RCC_SWS) != ((__RCC_CFGR_VAL<<2) & DV_RCC_SWS));

#endif
}
