/* tusb_os_custom.h - more OSALs for TinyUSB
 *
 * This file adds more OSALs to TinyUSB
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
*/
#ifndef tusb_os_custom_h
#define tusb_os_custom_h

#include "dv-config.h"

#ifdef DV_TUSB_OS_HDR
#include DV_TUSB_OS_HDR
#else
#error "DV_TUSB_OS_HDR is not defined"
#endif

#endif
