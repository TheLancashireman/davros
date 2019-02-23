/* davroska-osek.h - davroska header file for OSEK-compatibility
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_osek_h
#define dv_davroska_osek_h	1

#include <davroska.h>

/* StatusType has to be defined in conformance with the OSEK Binding Specification.
*/
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED   1
typedef unsigned char StatusType;
#define E_OK	0
#endif

/* Other standard OSEK types can just be defined
*/
typedef dv_id_t			TaskType;
typedef void *			TaskStateType;		/* Placeholder */
typedef dv_eventmask_t	EventMaskType;
typedef dv_id_t			ResourceType;
typedef dv_u64_t		TickType;
typedef dv_id_t			AlarmType;
typedef void *			AlarmBaseType;		/* Placeholder */
typedef dv_id_t			AppModeType;
typedef dv_sid_t		OSServiceIdType;

typedef TaskType *		TaskRefType;
typedef TaskStateType *	TaskStateRefType;
typedef EventMaskType *	EventMaskRefType;
typedef TickType *		TickRefType;
typedef AlarmBaseType *	AlarmBaseRefType;

enum dv_osekconst
{
	INVALID_TASK = -1,
	OSDEFAULTAPPMODE = 0,
	RUNNING	= 0,							/* Placeholder */
	WAITING = 0,							/* Placeholder */
	READY = 0,								/* Placeholder */
	SUSPENDED = 0,							/* Placeholder */
	E_OS_ACCESS = dv_e_access,
	E_OS_CALLEVEL = dv_e_calllevel,
	E_OS_ID = dv_e_id,
	E_OS_LIMIT = dv_e_limit,
	E_OS_NOFUNC = dv_e_nofunc,
	E_OS_RESOURCE = dv_e_resource,
	E_OS_STATE = dv_e_state,
	E_OS_VALUE = dv_e_value,
	DV_LAST = 0								/* Last */
};

#define DeclareTask(x)		extern TaskType x
#define DeclareResource(x)	extern ResourceType x
#define DeclareEvent(x)		extern EventMaskType x
#define DeclareAlarm(x)		extern AlarmType x

#define TASK(x)				void main_##x(void)
#define ISR(x)				void main_##x(void)
#define ALARMCALLBACK(x)	void acb_##x(void)

extern dv_intstatus_t dv_osek_disableallinterrupts_savedstate;
extern dv_id_t dv_osek_suspendallinterrupts_mutex;
extern dv_id_t dv_osek_suspendoslinterrupts_mutex;

/* OSEK-like "services"
*/
static inline StatusType ActivateTask(TaskType t)
{
	return (StatusType)dv_activatetask(t);
}

static inline StatusType ChainTask(TaskType t)
{
	return (StatusType)dv_chaintask(t);
}

static inline StatusType Schedule(void)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType TerminateTask(void)
{
	return (StatusType)dv_terminatetask();
}

static inline StatusType GetTaskID(TaskType *id_out)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType GetTaskState(TaskType t, TaskType *state_out)
{
	return (StatusType)dv_e_nofunc;
}

static inline void DisableAllInterrupts(void)
{
	dv_osek_disableallinterrupts_savedstate = dv_disable();
}

static inline void EnableAllInterrupts(void)
{
	(void)dv_restore(dv_osek_disableallinterrupts_savedstate);
}

static inline void SuspendAllInterrupts(void)
{
	(void)dv_takemutex(dv_osek_suspendallinterrupts_mutex);
}

static inline void ResumeAllInterrupts(void)
{
	(void)dv_dropmutex(dv_osek_suspendallinterrupts_mutex);
}

static inline void SuspendOSInterrupts(void)
{
	(void)dv_takemutex(dv_osek_suspendosinterrupts_mutex);
}

static inline void ResumeOSInterrupts(void)
{
	(void)dv_dropmutex(dv_osek_suspendosinterrupts_mutex);
}

static inline StatusType GetResource(ResourceType r)
{
	return (StatusType)dv_takemutex(r);
}

static inline StatusType ReleaseResource(ResourceType r)
{
	return (StatusType)dv_dropmutex(r);
}

static inline StatusType SetEvent(TaskType t, EventMaskType e)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType ClearEvent(EventMaskType e)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType GetEvent(TaskType t, EventMaskRefType e)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType WaitEvent(EventMaskType e)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType GetAlarmBase(AlarmType a, AlarmBaseRefType b)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType GetAlarm(AlarmType a, TickRefType t)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType SetAbsAlarm(AlarmType a, TickType t, TickType c)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType SetRelAlarm(AlarmType a, TickType t, TickType c)
{
	return (StatusType)dv_e_nofunc;
}

static inline StatusType CancelAlarm(AlarmType a)
{
	return (StatusType)dv_e_nofunc;
}

#endif
