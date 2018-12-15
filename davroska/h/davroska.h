/* davroska.h - internal header file for OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_h
#define dv_davroska_h	1

#ifndef DV_TARGET
/* No target defined, so assume we're compiling for a (linux) host
*/
#include <dv_target_linux.h>
#endif

#define	DV_NULL	0

typedef unsigned long long dv_param_t;

typedef enum
{
	dv_panic_none,
	dv_panic_ConfigNslotsInsufficient,
	dv_panic_QueueOverflow,
	dv_panic_CurrentExeCorrupt,
	dv_panic_CurrentExeDead,
	dv_panic_ReturnFromLongjmp
} dv_panic_t;

typedef struct dv_q_s
{
	dv_id_t *slots;			/* Start of a buffer for the ring buffer for this priority slot. */
    dv_qty_t nslots;		/* No. of slots in this ring buffer */
	dv_qty_t head;			/* Current position */
	dv_qty_t tail;			/* Current insertion position */
	dv_qty_t nq;			/* No of executables in queue */
    dv_prio_t prio;			/* Priority of this queue */
} dv_q_t;

typedef void (*dv_taskf_t)(void);

typedef struct dv_exe_s
{
	const char *name;
	dv_taskf_t func;
	dv_jmpbuf_t *jb;
	dv_qty_t maxact;
	dv_qty_t nact;
	dv_prio_t prio;
	dv_tstate_t state;
} dv_exe_t;

#endif
