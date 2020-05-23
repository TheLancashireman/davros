/* davroska.h - header file for OSEK-like member of the davros family
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
#ifndef dv_davroska_h
#define dv_davroska_h	1

#ifndef DV_ASM
#define DV_ASM	0
#endif

#include <dv-config.h>
#include DV_TARGET

/*==============================================================================
 *	davroska API
 *==============================================================================
*/
#define	DV_NULL	0

typedef short dv_id_t;
typedef short dv_prio_t;
typedef short dv_qty_t;
typedef dv_u64_t dv_eventmask_t;
typedef dv_u64_t dv_param_t;

typedef enum
{	dv_suspended,
	dv_ready,
	dv_running,
	dv_waiting
} dv_tstate_t;

typedef enum
{	dv_e_ok,			/* = 0; E_OK */
	dv_e_access,		/* = 1; first OSEK error code */
	dv_e_calllevel,
	dv_e_id,
	dv_e_limit,
	dv_e_nofunc,
	dv_e_resource,
	dv_e_state,
	dv_e_value,			/* = 8; last OSEK error code */
	dv_e_nesting,
	dv_e_reported,		/* Used when a lower layer has reported the error  */
	dv_w_value,
	dv_e_longjmp_ok		/* Must be last */
} dv_statustype_t;

typedef enum
{
	dv_sid_unknown,
	dv_sid_startos,
	dv_sid_terminatetask,
	dv_sid_activatetask,
	dv_sid_chaintask,
	dv_sid_takemutex,
	dv_sid_dropmutex,
	dv_sid_getexpirytime,
	dv_sid_getcountervalue,
	dv_sid_advancecounter,
	dv_sid_setalarm_abs,
	dv_sid_setalarm_rel,
	dv_sid_waitevent,
	dv_sid_setevent,
	dv_sid_getevent,
	dv_sid_clearevent,
	dv_sid_setrelalarm,		/* OSEK conformance */
	dv_sid_setabsalarm,		/* OSEK conformance */

	dv_sid_addtask,
	dv_sid_addextendedtask,
	dv_sid_addisr,
	dv_sid_startgroup,
	dv_sid_addtogroup,
	dv_sid_finishgroup,
	dv_sid_addmutex,
	dv_sid_addmutexuser,
	dv_sid_addexe,
	dv_sid_addcounter,
	dv_sid_addalarm,

	dv_sid_addosekcounter,
	dv_sid_addosekalarm_task,
	dv_sid_addosekalarm_acb,
	dv_sid_addosekalarm_counter,

	dv_sid_scheduler,
	dv_sid_interruptdispatcher,
	dv_sid_exceptionhandler,
	dv_sid_startup
} dv_sid_t;

typedef enum
{
	dv_panic_none,
	dv_panic_ConfigNslotsInsufficient,
	dv_panic_QueueOverflow,
	dv_panic_QueueCorrupt,
	dv_panic_CurrentExeCorrupt,
	dv_panic_CurrentExeDead,
	dv_panic_ReturnFromLongjmp,
	dv_panic_MutexOccupied,
	dv_panic_UnconfiguredInterrupt,
	dv_panic_IdleCreation,
	dv_panic_CounterOverflow,
	dv_panic_NoKernelAnchor,
	dv_panic_UnknownRequest,
	dv_panic_Exception,
	dv_panic_UnexpectedHardwareResponse,
	dv_panic_UndefinedPanic			/* Used as a temporary placeholder for new panics */
} dv_panic_t;

extern dv_statustype_t dv_startos(dv_id_t mode);
extern void dv_shutdown(dv_statustype_t reason);

extern dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);
extern dv_id_t dv_addextendedtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_u32_t stackbytes);
extern dv_id_t dv_addisr(const char *name, void (*fn)(void), dv_id_t irqid, dv_prio_t prio);
extern void dv_startgroup(const char *name, dv_boolean_t non_preemptable);
extern void dv_addtogroup(dv_id_t exe);
extern void dv_finishgroup(void);
extern dv_id_t dv_addmutex(const char *name, dv_qty_t maxtake);
extern void dv_addmutexuser(dv_id_t mutex, dv_id_t executable);
extern dv_id_t dv_addcounter(const char *name);
extern dv_id_t dv_addalarm(const char *name, dv_u64_t (*fn)(dv_id_t a, dv_param_t d), dv_param_t d);

extern dv_statustype_t dv_terminatetask(void);
extern dv_statustype_t dv_activatetask(dv_id_t task);
extern dv_statustype_t dv_chaintask(dv_id_t task);
extern dv_statustype_t dv_takemutex(dv_id_t mutex);
extern dv_statustype_t dv_dropmutex(dv_id_t mutex);

extern dv_statustype_t dv_setalarm_rel(dv_id_t c, dv_id_t a, dv_u64_t v);
extern dv_statustype_t dv_setalarm_abs(dv_id_t c, dv_id_t a, dv_u64_t v);
extern dv_statustype_t dv_advancecounter(dv_id_t c, dv_u64_t n);
extern dv_statustype_t dv_stopalarm(dv_id_t a);
extern dv_u64_t dv_getexpirytime(dv_id_t a);
extern dv_u64_t dv_getcountervalue(dv_id_t c);

/* Configuration callout functions: provided by the application
*/
extern void callout_addtasks(dv_id_t mode);
extern void callout_addisrs(dv_id_t mode);
extern void callout_addgroups(dv_id_t mode);
extern void callout_addmutexes(dv_id_t mode);
extern void callout_addcounters(dv_id_t mode);
extern void callout_addalarms(dv_id_t mode);
extern void callout_autostart(dv_id_t mode);

/* Runtime callout functions: provided by the application
*/
extern void callout_startup(void);
extern void callout_preexe(void);
extern void callout_postexe(void);
extern dv_statustype_t callout_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nParam, dv_param_t *p);
extern void callout_panic(dv_panic_t p, dv_sid_t sid, char *fault);
extern void callout_shutdown(dv_statustype_t e);
extern void callout_idle(void);

#if DV_CFG_MAXEXTENDED <= 0
/* Event API in "BCC1/BCC2" is just a stub. All functions return dv_e_access, even if task ID is out of range.
*/
static inline dv_statustype_t dv_waitevent(dv_eventmask_t evts)
{
	dv_param_t p = (dv_param_t)evts;
	return callout_reporterror(dv_e_access, dv_sid_waitevent, 1, &p);
}

static inline dv_statustype_t dv_setevent(dv_id_t t, dv_eventmask_t evts)
{
	dv_param_t p[2];
	p[0] = (dv_param_t)t;
	p[1] = (dv_param_t)evts;
	return callout_reporterror(dv_e_access, dv_sid_setevent, 2, p);
}

static inline dv_statustype_t dv_getevent(dv_id_t t, dv_eventmask_t *evts)
{
	dv_param_t p[2];
	p[0] = (dv_param_t)t;
	p[1] = (dv_param_t)(dv_address_t)evts;
	return callout_reporterror(dv_e_access, dv_sid_getevent, 2, p);
}

static inline dv_statustype_t dv_clearevent(dv_eventmask_t evts)
{
	dv_param_t p = (dv_param_t)evts;
	return callout_reporterror(dv_e_access, dv_sid_clearevent, 1, &p);
}
#else
/* Proper event API is provided in davroska-extended.c
*/
extern dv_statustype_t dv_waitevent(dv_eventmask_t evts);
extern dv_statustype_t dv_setevent(dv_id_t t, dv_eventmask_t evts);
extern dv_statustype_t dv_getevent(dv_id_t t, dv_eventmask_t *evts);
extern dv_statustype_t dv_clearevent(dv_eventmask_t evts);
#endif

/*==============================================================================
 *	davroska internals
 *==============================================================================
*/
/* DV_CANARY - no. of bytes of space above stack top and below stack limit for over/underflow detection
 *
 * Also use for stack rounding, so it's useful if DV_CANARY is the size of a cache line.
 * Must be a power of 2
*/
#ifndef DV_CANARY
#define DV_CANARY	32
#endif

#if ((DV_CANARY) & ((DV_CANARY)-1)) != 0
#error "DV_CANARY is not a power of 2"
#endif

typedef enum
{
	ph_addtasks,
	ph_addisrs,
	ph_addgroups,
	ph_addmutexes,
	ph_addcounters,
	ph_addalarms,
	ph_addosekcounters,
	ph_addosekalarms
} dv_cfgphase_t;

typedef struct dv_configstate_s
{
	dv_cfgphase_t phase;
	void *data;
} dv_configstate_t;

typedef struct dv_q_s
{
	dv_id_t *slots;			/* Start of a buffer for the ring buffer for this priority slot. */
    dv_qty_t nslots;		/* No. of slots in this ring buffer */
	dv_qty_t head;			/* Current position */
	dv_qty_t tail;			/* Current insertion position */
	dv_qty_t nq;			/* No of executables in queue */
    dv_prio_t prio;			/* Priority of this queue */
    dv_prio_t level;		/* Interrupt lock level of this queue */
} dv_q_t;

typedef struct dv_extended_s
{
	dv_u64_t events_pending;
	dv_u64_t events_awaited;
	dv_jmpbuf_t *jb;
	dv_stackword_t *stackbase;
	dv_stackword_t *initialsp;
	dv_u32_t stacklen;
} dv_extended_t;

typedef void (*dv_taskf_t)(void);

typedef struct dv_exe_s
{
	const char *name;
	dv_taskf_t func;
	dv_jmpbuf_t *jb;
	dv_qty_t maxact;
	dv_qty_t nact;
	dv_prio_t baseprio;
	dv_prio_t runprio;
	dv_prio_t currprio;
	dv_tstate_t state;
	dv_id_t mutexlist;
	dv_id_t irqid;
	dv_id_t extended;
} dv_exe_t;

typedef struct dv_mutex_s
{
	const char *name;
	dv_prio_t ceiling;
	dv_qty_t maxtake;
	dv_qty_t ntake;
	dv_id_t owner;
	dv_prio_t saveprio;
	dv_id_t next;
} dv_mutex_t;

typedef struct dv_counter_s
{
	const char *name;
	dv_u64_t value;
	dv_id_t head;
} dv_counter_t;

typedef struct dv_alarm_s
{
	const char *name;
	dv_u64_t expirytime;
	dv_u64_t (*expiryfunction)(dv_id_t a, dv_param_t d);
	dv_param_t expiryfunctiondata;
	dv_id_t counter;
	dv_id_t nextalarm;
} dv_alarm_t;

typedef struct dv_params_runqueued_s
{
	dv_prio_t high;
	dv_prio_t low;
	dv_intstatus_t is;
} dv_params_runqueued_t;

typedef enum
{	dv_req_rq,
	dv_req_xx
} dv_req_t;

typedef struct dv_request_s
{
	dv_req_t code;
	dv_jmpbuf_t *return_jb;
	union
	{	dv_params_runqueued_t rq;
	} p;
} dv_request_t;

typedef struct dv_errorinfo_s
{
	dv_sid_t sid;
	dv_statustype_t e;
	dv_param_t p[4];
	dv_id_t culprit;
} dv_errorinfo_t;

typedef struct dv_softvector_s
{
	dv_statustype_t (*fn)(dv_id_t p);
	dv_id_t p;
} dv_softvector_t;

/* Task (etc.) management variables
*/
extern dv_configstate_t *dv_configstate;

extern const dv_qty_t dv_maxexe;
extern const dv_qty_t dv_maxextended;
extern const dv_qty_t dv_maxprio;
extern const dv_qty_t dv_maxslot;
extern const dv_qty_t dv_maxmutex;

extern dv_prio_t dv_highprio;
extern dv_prio_t dv_highestprio;
extern dv_id_t dv_currentexe;

extern dv_qty_t dv_nexe;
extern dv_qty_t dv_nextended;
extern dv_qty_t dv_ntask;
extern dv_qty_t dv_nisr;
extern dv_qty_t dv_nmutex;

extern dv_prio_t dv_maxtaskprio;

extern dv_exe_t dv_exe[];
extern dv_extended_t dv_extended[];
extern dv_q_t dv_queue[];
extern dv_id_t dv_slots[];
extern dv_mutex_t dv_mutex[];
extern dv_softvector_t dv_vectors[];

/* Counter (etc.) management variables
*/
extern const dv_qty_t dv_maxcounter;
extern const dv_qty_t dv_maxalarm;

extern dv_qty_t dv_ncounter;
extern dv_qty_t dv_nalarm;

extern void dv_runqueued(dv_prio_t high, dv_prio_t low, dv_intstatus_t is);
extern void dv_panic(dv_panic_t p, dv_sid_t sid, char *fault);
extern dv_statustype_t dv_unconfigured_interrupt(dv_id_t p);
extern dv_prio_t dv_raiseprio(void);
extern void dv_lowerprio(dv_prio_t p);

extern void dv_setalarm(dv_id_t c, dv_id_t a, dv_u64_t v);

/* The following prototypes are not needed if there's no extended task support.
 * The functions are either inline stubs or not required at all
*/
#if DV_CFG_MAXEXTENDED > 0
extern void dv_runextended(dv_id_t e, dv_intstatus_t is);
extern void dv_runqueued_onkernelstack(dv_prio_t high, dv_prio_t low, dv_intstatus_t is);
extern void dv_startextendedtask(dv_id_t e, dv_intstatus_t is);
extern void dv_switchcall(dv_id_t e, dv_intstatus_t is, dv_stackword_t *sp, void (*f)(dv_id_t e, dv_intstatus_t is));
extern void dv_extended_init(dv_stackword_t *stackbase);
#endif


#endif
