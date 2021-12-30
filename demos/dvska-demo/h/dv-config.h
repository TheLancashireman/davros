/* dv-config.h - configuration header file for a davroska system
 *
 * This file contains the configuration parameters for the davroska kernel used in this demo
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
#ifndef dv_config_h
#define dv_config_h	1

#define DEMO_PI_ZERO	1
#define DEMO_PI3_ARM64	2
#define DEMO_BLUE_PILL	3
#define DEMO_PI_PICO	4

#if DEMO_BOARD == DEMO_PI_ZERO

#define DV_TARGET			<arm/h/dv-target-pi-zero.h>
#define DV_DEMO_TARGET		<demo-pi-zero.h>

#elif DEMO_BOARD == DEMO_PI3_ARM64

#define DV_TARGET			<arm64/h/dv-target-pi3-arm64.h>
#define DV_DEMO_TARGET		<demo-pi3-arm64.h>
#define DV_TARGET_CORE		0

#elif DEMO_BOARD == DEMO_BLUE_PILL

#define DV_TARGET			<arm/h/dv-target-blue-pill.h>
#define DV_DEMO_TARGET		<demo-blue-pill.h>

#elif DEMO_BOARD == DEMO_PI_PICO

#define DV_TARGET			<arm/h/dv-target-pi-pico.h>
#define DV_DEMO_TARGET		<demo-pi-pico.h>
#define DV_TARGET_CORE		0

#else
#error "DEMO_BOARD not known"
#endif

/* DV_CFG_MAXEXE is the maximum number of executables (tasks + ISRs) that you can create.
*/
#define DV_CFG_MAXEXE		20

/* DV_CFG_MAXEXTENDED is the maximum number of extended executables that you can create.
 * If this is zero, the event API is omitted.
*/
#define DV_CFG_MAXEXTENDED	5

/* DV_CFG_MAXPRIO is the maximum number of different priorities that you can use
 * The number includes all task and ISR priorities
*/
#define DV_CFG_MAXPRIO		10

/* DV_CFG_NSLOT_EXTRA is the number of extra queue slots you need for tasks with maxact > 1
 * If this is zero the "multiple activations" feature is omitted,
*/
#define DV_CFG_NSLOT_EXTRA	10

/* DV_CFG_MAXMUTEX is the maximum number of mutexes that you can create
*/
#define DV_CFG_MAXMUTEX		10

/* DV_CFG_MAXCOUNTER is the maximum number of counters that you can create
*/
#define DV_CFG_MAXCOUNTER	5

/* DV_CFG_MAXALARM is the maximum number of alarms that you can create
*/
#define DV_CFG_MAXALARM		10

/* DV_CFG_MAXALARM_OSEK is the maximum number of OSEK alarms that you can create
*/
#define DV_CFG_MAXALARM_OSEK	5

/* DV_CFG_MAXCOUNTER_OSEK is the maximum number of OSEK counters that you can create
*/
#define DV_CFG_MAXCOUNTER_OSEK	5

/* Inline versions of the empty callout functions
*/
static inline void callout_preexe(void)
{
}

static inline void callout_postexe(void)
{
}

static inline void callout_startup(void)
{
}


#endif
