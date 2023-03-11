/* tusb_config.h - configuration header file for tinyusb
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/
#ifndef tusb_config_h
#define tusb_config_h

#include "dv-config.h"

#if DEMO_BOARD == DEMO_PI_ZERO
#error "No tinyusb support for pi-zero"
#elif DEMO_BOARD == DEMO_PI3_ARM64
#error "No tinyusb support for pi3"
#elif DEMO_BOARD == DEMO_BLUE_PILL
#define CFG_TUSB_MCU	OPT_MCU_STM32F1
#define STM32F103x6		1
#elif DEMO_BOARD == DEMO_PI_PICO
#define CFG_TUSB_MCU	OPT_MCU_RP2040
#else
#error "DEMO_BOARD not known"
#endif

#define CFG_TUSB_OS 			OPT_OS_CUSTOM

#include <tusb_config_cdc.h>

#endif
