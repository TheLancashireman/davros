/* davroska.h - internal header file for OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_h
#define dv_davroska_h	1

#ifdef DV_TARGET
#include DV_TARGET
#else
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
	dv_panic_QueueCorrupt,
	dv_panic_CurrentExeCorrupt,
	dv_panic_CurrentExeDead,
	dv_panic_ReturnFromLongjmp,
	dv_panic_LockOccupied
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
	dv_prio_t baseprio;
	dv_prio_t currprio;
	dv_tstate_t state;
	dv_id_t locklist;
} dv_exe_t;

typedef struct dv_lock_s
{
	const char *name;
	dv_prio_t ceiling;
	dv_qty_t maxtake;
	dv_qty_t ntake;
	dv_id_t owner;
	dv_prio_t saveprio;
	dv_id_t next;
} dv_lock_t;

typedef struct dv_errorinfo_s
{
	dv_sid_t sid;
	dv_statustype_t e;
	dv_param_t p[4];
	dv_id_t culprit;
} dv_errorinfo_t;

#endif
