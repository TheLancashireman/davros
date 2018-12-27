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
	dv_sid_droplock
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

dv_statustype_t dv_startos(dv_id_t mode);

dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);
dv_id_t dv_addisr(const char *name, void (*fn)(void), dv_id_t irqid, dv_prio_t prio);
dv_id_t dv_addlock(const char *name, dv_qty_t maxtake);
void dv_addlockuser(dv_id_t lock, dv_id_t executable);

dv_statustype_t dv_terminatetask(void);
dv_statustype_t dv_activatetask(dv_id_t task);
dv_statustype_t dv_chaintask(dv_id_t task);
dv_statustype_t dv_takelock(dv_id_t lock);
dv_statustype_t dv_droplock(dv_id_t lock);

/* Configuration callout functions: provided by the application
*/
void callout_addtasks(dv_id_t mode);
void callout_addisrs(dv_id_t mode);
void callout_addlocks(dv_id_t mode);
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

extern void dv_runqueued(dv_prio_t high, dv_prio_t low, dv_intstatus_t is);
extern void dv_panic(dv_panic_t p);
extern dv_statustype_t dv_unconfigured_interrupt(dv_id_t p);

#endif
