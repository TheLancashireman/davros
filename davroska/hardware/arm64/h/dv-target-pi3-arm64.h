/* dv-target-pi3-arm64.h - header file for davroska on a pi3 in 64-bit mode
 *
 * (c) David Haworth
*/
#ifndef dv_target_pi3_arm64_h
#define dv_target_pi3_arm64_h	1

#include <arm64/h/dv-types-arm64.h>
#include <dv-stdio.h>

#define DV_PBASE	0x3f000000
#define DV_PBASE2	0x40000000

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-arm-bcm2836-interruptcontroller.h>
#define DV_SUPPORT_INTLEVEL	1
#define DV_MAX_INTLEVEL		7
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL+1)
#define DV_NVECTOR			(dv_n_bcm2836_iid)

#endif
