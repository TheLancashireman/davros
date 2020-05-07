/* os.h -  "generated" OS header file
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
#ifndef os_h
#define os_h
#include <dv-config.h>
#include <davroska-osek.h>

/* Object identifiers
*/
extern dv_id_t Uart, Timer;		/* ISRs */
extern dv_id_t Ticker;			/* Davroska counters */
extern dv_id_t OsekCounter;		/* OSEK counters */
extern const EventMaskType ev_Flicker;
extern const EventMaskType ev_Update;

extern TASK(Init);
extern TASK(Led);
extern TASK(Bit0);
extern TASK(Bit1);
extern TASK(Bit2);
extern TASK(Bit3);
extern ISR(Uart);
extern ISR(Timer);

#endif
