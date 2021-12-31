/* demo.c - demo for davroska (OSEK version)
 *
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
 * This file contains all the target-independent stuff - tasks, ISRs etc.
 * The target-dependent stuff (startup, main, timer & uart functions) can be found in
 * hardware specific header/source files.
 *
 * (c) David Haworth
*/
#ifndef demo_h
#define demo_h

#define USE_USB		1

#include "tusb.h"

void send_hid_report(uint8_t report_id, uint32_t btn);

#define REPORTID_KEYBOARD			1
#if 0
#define REPORTID_MOUSE				2
#define REPORTID_CONSUMER_CONTROL	3
#define REPORTID_GAMEPAD			4
#endif
#define REPORTID_LAST				1

#endif
