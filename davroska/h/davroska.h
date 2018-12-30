/* davroska.h - header file for OSEK-like member of the davros family
 *
 * (c) David Haworth
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
	dv_e_longjmp_ok		/* Must be last */
} dv_statustype_t;

typedef enum
{
	dv_sid_terminatetask,
	dv_sid_activatetask,
	dv_sid_chaintask,
	dv_sid_takelock,
	dv_sid_droplock,
	dv_sid_getexpirytime,
	dv_sid_advancecounter,
	dv_sid_setalarm_abs,
	dv_sid_setalarm_rel,
	dv_sid_waitevent,
	dv_sid_setevent,
	dv_sid_getevent,
	dv_sid_clearevent
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
	dv_panic_LockOccupied,
	dv_panic_UnconfiguredInterrupt,
	dv_panic_UnknownPanic
} dv_panic_t;

extern dv_statustype_t dv_startos(dv_id_t mode);

extern dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);
extern dv_id_t dv_addisr(const char *name, void (*fn)(void), dv_id_t irqid, dv_prio_t prio);
extern dv_id_t dv_addlock(const char *name, dv_qty_t maxtake);
extern dv_id_t dv_addcounter(const char *name);
extern dv_id_t dv_addalarm(const char *name, dv_u32_t (*fn)(dv_id_t a));
extern void dv_addlockuser(dv_id_t lock, dv_id_t executable);

extern dv_statustype_t dv_terminatetask(void);
extern dv_statustype_t dv_activatetask(dv_id_t task);
extern dv_statustype_t dv_chaintask(dv_id_t task);
extern dv_statustype_t dv_takelock(dv_id_t lock);
extern dv_statustype_t dv_droplock(dv_id_t lock);

extern dv_statustype_t dv_setalarm_rel(dv_id_t c, dv_id_t a, dv_u32_t v);
extern dv_statustype_t dv_setalarm_abs(dv_id_t c, dv_id_t a, dv_u64_t v);
extern dv_statustype_t dv_advancecounter(dv_id_t c, dv_u64_t n);

/* ToDo: this will become an application callout to replace error callout
*/
extern dv_statustype_t dv_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nParam, dv_param_t *p);

#if DV_CFG_MAXEXTENDED <= 0
/* Event API in "BCC1/BCC2" is just a stub. All functions return dv_e_access, even if task ID is out of range.
*/
static inline dv_statustype_t dv_waitevent(dv_eventmask_t evts)
{
	dv_param_t p = (dv_param_t)evts;
	return dv_reporterror(dv_e_access, dv_sid_waitevent, 1, &p);
}

static inline dv_statustype_t dv_setevent(dv_id_t t, dv_eventmask_t evts)
{
	dv_param_t p[2];
	p[0] = (dv_param_t)t;
	p[1] = (dv_param_t)evts;
	return dv_reporterror(dv_e_access, dv_sid_setevent, 2, p);
}

static inline dv_statustype_t dv_getevent(dv_id_t t, dv_eventmask_t *evts)
{
	dv_param_t p[2];
	p[0] = (dv_param_t)t;
	p[1] = (dv_param_t)(dv_address_t)evts;
	return dv_reporterror(dv_e_access, dv_sid_getevent, 2, p);
}

static inline dv_statustype_t dv_clearevent(dv_eventmask_t evts)
{
	dv_param_t p = (dv_param_t)evts;
	return dv_reporterror(dv_e_access, dv_sid_clearevent, 1, &p);
}
#else
/* Proper event API is provided in davroska-extended.c
*/
extern dv_statustype_t dv_waitevent(dv_eventmask_t evts);
extern dv_statustype_t dv_setevent(dv_id_t t, dv_eventmask_t evts);
extern dv_statustype_t dv_getevent(dv_id_t t, dv_eventmask_t *evts);
extern dv_statustype_t dv_clearevent(dv_eventmask_t evts);
extern dv_id_t dv_addextendedtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_u32_t stackbytes);
#endif

/* Configuration callout functions: provided by the application
*/
void callout_addtasks(dv_id_t mode);
void callout_addisrs(dv_id_t mode);
void callout_addlocks(dv_id_t mode);
void callout_addcounters(dv_id_t mode);
void callout_addalarms(dv_id_t mode);
void callout_autostart(dv_id_t mode);

/* Runtime callout functions: provided by the application
*/
void callout_startup(void);
void callout_preexe(void);
void callout_postexe(void);
void callout_error(dv_statustype_t e);
void callout_shutdown(dv_statustype_t e);

/*==============================================================================
 *	davroska internals
 *==============================================================================
*/
#define DV_CANARY	32		/* Amount of space above stack top and below stack limit for over/underflow detection */

typedef unsigned long long dv_param_t;

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

typedef struct dv_extexe_s
{
	dv_u64_t events_pending;
	dv_u64_t events_awaited;
	dv_u32_t *initialsp;
	dv_u32_t stack[(DV_CANARY*2)/4];
} dv_extexe_t;

typedef void (*dv_taskf_t)(void);

typedef struct dv_exe_s
{
	const char *name;
	dv_taskf_t func;
	dv_extexe_t *extended;
	dv_jmpbuf_t *jb;
	dv_qty_t maxact;
	dv_qty_t nact;
	dv_prio_t baseprio;
	dv_prio_t runprio;
	dv_prio_t currprio;
	dv_tstate_t state;
	dv_id_t locklist;
	dv_id_t irqid;
} dv_exe_t;

typedef struct dv_lock_s
{
	const char *name;
	dv_prio_t ceiling;
	dv_prio_t locklevel;
	dv_qty_t maxtake;
	dv_qty_t ntake;
	dv_id_t owner;
	dv_prio_t saveprio;
	dv_id_t next;
} dv_lock_t;

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
	dv_u32_t (*expiryfunction)(dv_id_t a);
	dv_id_t counter;
	dv_id_t nextalarm;
} dv_alarm_t;
	

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
extern dv_prio_t dv_highestprio;
extern dv_id_t dv_currentexe;

extern const dv_qty_t dv_maxexe;
extern const dv_qty_t dv_maxprio;
extern const dv_qty_t dv_maxslot;
extern const dv_qty_t dv_maxlock;

extern dv_prio_t dv_highprio;
extern dv_prio_t dv_highestprio;
extern dv_id_t dv_currentexe;

extern dv_qty_t dv_nexe;
extern dv_qty_t dv_ntask;
extern dv_qty_t dv_nisr;
extern dv_qty_t dv_nlock;

extern dv_prio_t dv_maxtaskprio;

extern dv_exe_t dv_exe[];
extern dv_q_t dv_queue[];
extern dv_id_t dv_slots[];
extern dv_lock_t dv_lock[];
extern dv_softvector_t dv_vectors[];

/* Counter (etc.) management variables
*/
extern const dv_qty_t dv_maxcounter;
extern const dv_qty_t dv_maxalarm;

extern dv_qty_t dv_ncounter;
extern dv_qty_t dv_nalarm;

extern void dv_runqueued(dv_prio_t high, dv_prio_t low, dv_intstatus_t is);
extern void dv_panic(dv_panic_t p);
extern dv_statustype_t dv_unconfigured_interrupt(dv_id_t p);
extern dv_prio_t dv_raiseprio(void);
extern void dv_lowerprio(dv_prio_t p);

#endif
