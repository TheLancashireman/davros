/* dv-target-blue-pill.h - header file for davroska on a Blue Pill
 *
 * (c) David Haworth
*/
#ifndef dv_target_blue_pill_h
#define dv_target_blue_pill_h	1

#include <arm/h/dv-types-cortexm.h>
#include <dv-cortex-m3.h>
#include <dv-stdio.h>

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-nvic.h>
#define DV_MAX_INTLEVEL		14
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL+1)
#define DV_NVECTOR			68

#endif
