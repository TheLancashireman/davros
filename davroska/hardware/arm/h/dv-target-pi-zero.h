/* dv-target-pi-zero.h - header file for davroska on a pi-zero
 *
 * (c) David Haworth
*/
#ifndef dv_target_pi_zero_h
#define dv_target_pi_zero_h	1

#include <arm/h/dv-types-arm.h>
#include <dv-stdio.h>

#define DV_PBASE	0x20000000

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-arm-bcm2835-interruptcontroller.h>
#define DV_SUPPORT_INTLEVEL	1
#define DV_MAX_INTLEVEL		7
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL+1)
#define DV_NVECTOR			dv_n_bcm2835_iid

#endif
