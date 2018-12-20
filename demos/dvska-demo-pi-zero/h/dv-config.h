/* dv-config.h - configuration header file for a davroska system
 *
 * This file contains the configuration parameters for the davroska kernel used in this demo
 *
 * (c) David Haworth
*/
#ifndef dv_config_h
#define dv_config_h	1

#define DV_TARGET			<arm/h/dv-target-pi-zero.h>

/* DV_CFG_MAXEXE is the maximum number of executables that you can create.
*/
#define DV_CFG_MAXEXE		20

/* DV_CFG_MAXPRIO is the maximum number of different priorities that you can have
*/
#define DV_CFG_MAXPRIO		10

/* DV_CFG_NSLOT_EXTRA is the number of extra queue slots you need for tasks with maxact > 1
*/
#define DV_CFG_NSLOT_EXTRA	10

/* DV_CFG_MAXLOCK is the maximum number of locks that you can have
*/
#define DV_CFG_MAXLOCK		10

#endif
