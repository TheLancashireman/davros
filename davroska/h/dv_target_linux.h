/* dv_target_linux.h - header file for linux-hosted davroska
 *
 * (c) David Haworth
*/
#ifndef dv_target_linux_h
#define dv_target_linux_h	1

#include <stdio.h>
#include <setjmp.h>

typedef unsigned dv_intstatus_t;

typedef jmp_buf dv_jmpbuf_t;

#define dv_setjmp	setjmp
#define dv_longjmp	longjmp
#define dv_printf	printf

/* Cannot disable/restore interrupts on Linux
*/
#define DV_INTENABLED	0

static inline dv_intstatus_t dv_disable()
{
	return 0;
}

static inline dv_intstatus_t dv_restore(dv_intstatus_t unused_x)
{
	return 0;
}

#endif
