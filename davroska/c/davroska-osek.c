/* davroska-osek.c - functions and data for OSEK compatibility
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <davroska-osek.h>

/* OSEK counters and alarms
*/
struct dv_osekcounter_s dv_osekcounter[DV_CFG_MAXCOUNTER_OSEK];
struct dv_osekalarm_s dv_osekalarm[DV_CFG_MAXALARM_OSEK];

dv_qty_t dv_nosekcounter;
dv_qty_t dv_nosekalarm;

const dv_qty_t dv_maxosekcounter = DV_CFG_MAXCOUNTER_OSEK;
const dv_qty_t dv_maxosekalarm = DV_CFG_MAXALARM_OSEK;

/* OSEK last error information
*/
struct dv_lasterror_s dv_lasterror;

/* OSEK error reporting
 *
 * ToDo: make lasterror information and call to ErrorHook optional (configurable)
*/
dv_statustype_t callout_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nParam, dv_param_t *p)
{
	if ( dv_lasterror.inerrorhook )		/* No ErrorHook nesting */
		return e;

	dv_lasterror.e = e;
	dv_lasterror.sid = sid;
	if ( nParam > DV_MAXPARAM )
		nParam = DV_MAXPARAM;
	dv_lasterror.n_param = nParam;
	for ( int i = 0; i < nParam; i++ )
		dv_lasterror.p[i] = p[i];

	dv_lasterror.inerrorhook = 1;
	ErrorHook(e);
	dv_lasterror.inerrorhook = 0;

	return e;
}

/* dv_checkalarmparams() - common function to check SetXxxAlarm() parameters
*/
static inline StatusType dv_checkalarmparams(AlarmType a, TickType t, TickType c, dv_sid_t sid)
{
	if ( (a < 0) && (a > dv_nalarm) )
	{
		/* Alarm does not exist
		*/
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
		return (StatusType)callout_reporterror(sid, dv_e_id, 3, p);
	}

	dv_id_t ctr = dv_osekalarm[a].osekcounter;

	if ( ctr < 0 )
	{
		/* Alarm has not been mapped to a counter (i.e. isn't an OSEK alarm)
		*/
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
		return (StatusType)callout_reporterror(sid, dv_e_id, 3, p);
	}

	if ( t > dv_osekcounter[ctr].maxvalue )
	{
		/* Initial count too large
		*/
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
		return (StatusType)callout_reporterror(sid, dv_e_value, 3, p);
	}

	if ( c != 0 )	/* c == 0 is special case for "single shot" */
	{
		if ( c > dv_osekcounter[ctr].maxvalue )
		{
			/* Cycle count too large
			*/
			dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
			return (StatusType)callout_reporterror(sid, dv_e_value, 3, p);
		}
		else if ( c < dv_osekcounter[ctr].mincycle )
		{
			/* Cycle count too small
			*/
			dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
			return (StatusType)callout_reporterror(sid, dv_e_value, 3, p);
		}
	}

	if ( dv_getexpirytime(a) != 0 )
	{
		/* Alarm is already in use
		*/
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)t, (dv_param_t)c};
		return (StatusType)callout_reporterror(sid, dv_e_state, 3, p);
	}

	return dv_e_ok;
}

/* SetRelAlarm() - OSEK-conformant service
 *
 * Prerequisite: the alarm has been configured as an OSEK alarm
 *
 * Setting an alarm relative to the current counter value is much the same as
 * the internal davroska function.
 *
 * ToDo: extract the common parts into a common function
*/
StatusType SetRelAlarm(AlarmType a, TickType tim, TickType cyc)
{
	StatusType e = dv_checkalarmparams(a, tim, cyc, dv_sid_setrelalarm);
	if ( e != dv_e_ok )
		return e;

	dv_intstatus_t is = dv_disable();

	dv_id_t c = dv_osekcounter[dv_osekalarm[a].osekcounter].counter;
	dv_u64_t curval = dv_getcountervalue(c);
	dv_u64_t absval = curval + tim;

	if ( absval < curval )
	{
		/* Calculation has overflowed
		*/
		dv_restore(is);
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)tim, (dv_param_t)cyc};
		return (StatusType)callout_reporterror(dv_sid_setrelalarm, dv_e_value, 3, p);
	}

	dv_osekalarm[a].cycle = cyc;
	dv_setalarm(c, a, absval);

	dv_restore(is);

	return dv_e_ok;
}

/* SetAbsAlarm() - OSEK-conformant service
 *
 * Prerequisite: the alarm has been configured as an OSEK alarm
 *
 * Setting an alarm at an absolute value of the OSEK counter is fun ;-)
 * Recall that the OSEK counter can "wrap around" at a configured value.
*/
StatusType SetAbsAlarm(AlarmType a, TickType tim, TickType cyc)
{
	StatusType e = dv_checkalarmparams(a, tim, cyc, dv_sid_setabsalarm);
	if ( e != dv_e_ok )
		return e;

	dv_intstatus_t is = dv_disable();

	dv_id_t c = dv_osekcounter[dv_osekalarm[a].osekcounter].counter;
	dv_u64_t curval = dv_getcountervalue(c);

	dv_u64_t modulus = dv_osekcounter[dv_osekalarm[a].osekcounter].maxvalue + 1;
	dv_u64_t curosek = curval % modulus;

	dv_u64_t absval = curval + tim - curosek;
	if ( curosek >= tim )
		absval += modulus;

	if ( absval < curval )
	{
		/* Calculation has overflowed
		*/
		dv_restore(is);
		dv_param_t p[3] = {(dv_param_t)a, (dv_param_t)tim, (dv_param_t)cyc};
		return (StatusType)callout_reporterror(dv_sid_setrelalarm, dv_e_value, 3, p);
	}

	dv_osekalarm[a].cycle = cyc;
	dv_setalarm(c, a, absval);

	dv_restore(is);

	return dv_e_ok;
}

/* dv_osekinit() - initialise all the OSEK compatibility data structures
 *
 * Initialises the non-zero initial values of the OSEK-to-davroska mapping structures
*/
void dv_osekinit(void)
{
	for ( int i = 0; i < DV_CFG_MAXALARM_OSEK; i++ )
	{
		dv_osekalarm[i].osekcounter = -1;
	}

	for ( int i = 0; i < DV_CFG_MAXCOUNTER_OSEK; i++ )
	{
		dv_osekcounter[i].counter = -1;
	}

	dv_lasterror.e = dv_e_ok;
	dv_lasterror.sid = dv_sid_unknown;
}

static dv_id_t dv_addosekalarm(const char *name, dv_id_t c, dv_u64_t (*af)(dv_id_t a, dv_param_t d));
static dv_u64_t dv_osekaf_activatetask(dv_id_t a, dv_param_t d);
static dv_u64_t dv_osekaf_setevent(dv_id_t a, dv_param_t d);
static dv_u64_t dv_osekaf_callacb(dv_id_t a, dv_param_t d);
static dv_u64_t dv_osekaf_incrementcounter(dv_id_t a, dv_param_t d);

/* dv_addosekalarm_task() - add an OSEK alarm to the configuration
 *
 * Configures an alarm that activates a task or sets an event for a task
*/
dv_id_t dv_addosekalarm_task(const char *name, dv_id_t c, dv_id_t t, dv_eventmask_t e)
{
	if ( (t < 0) || (t >= dv_ntask) )
	{
		dv_param_t p[4];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)t;
		p[3] = (dv_param_t)e;
		callout_reporterror(dv_sid_addosekalarm_task, dv_e_id, 4, p);
		return -1;
	}

	dv_id_t id; 

	if ( e == 0 )
		id = dv_addosekalarm(name, c, dv_osekaf_activatetask);
	else
		id = dv_addosekalarm(name, c, dv_osekaf_setevent);

	if ( id < 0 )
	{
		dv_param_t p[4];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)t;
		p[3] = (dv_param_t)e;
		callout_reporterror(dv_sid_addosekalarm_task, -id, 4, p);
		return -1;
	}

	dv_osekalarm[id].obj = t;
	dv_osekalarm[id].more.e = e;

	return id;
}

/* dv_addosekalarm_acb() - add an OSEK alarm to the configuration
 *
 * Configures an alarm that invokes an alarm callback function
*/
dv_id_t dv_addosekalarm_acb(const char *name, dv_id_t c, void (*acb)(void))
{
	dv_id_t id = dv_addosekalarm(name, c, dv_osekaf_callacb);

	if ( id < 0 )
	{
		dv_param_t p[3];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)(dv_address_t)acb;
		callout_reporterror(dv_sid_addosekalarm_acb, -id, 3, p);
		return -1;
	}

	dv_osekalarm[id].obj = -1;
	dv_osekalarm[id].more.acb = acb;

	return id;
}

/* dv_addosekalarm_counter() - add an OSEK alarm to the configuration
 *
 * Configures an alarm that increments another counter
*/
dv_id_t dv_addosekalarm_counter(const char *name, dv_id_t c, dv_id_t c2incr)
{
	if ( (c2incr < 0) || (c2incr >= dv_ncounter) )
	{
		dv_param_t p[3];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)c2incr;
		callout_reporterror(dv_sid_addosekalarm_counter, dv_e_id, 3, p);
		return -1;
	}

	dv_id_t id = dv_addosekalarm(name, c, dv_osekaf_incrementcounter);

	if ( id < 0 )
	{
		dv_param_t p[3];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)c2incr;
		callout_reporterror(dv_sid_addosekalarm_counter, -id, 3, p);
		return -1;
	}

	dv_osekalarm[id].obj = c2incr;
	dv_osekalarm[id].more.e = 0;

	return id;
}

/* dv_addosekcounter() - add an OSEK counter to the configuration
 *
*/
dv_id_t dv_addosekcounter(const char *name, dv_id_t c, dv_u64_t max, dv_u64_t min)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addosekcounters) )
	{
		dv_param_t p[4];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)max;
		p[3] = (dv_param_t)min;
		callout_reporterror(dv_sid_addosekcounter, dv_e_calllevel, 4, p);
		return -1;
	}

	if ( dv_nosekcounter >= dv_maxosekcounter )
	{
		dv_param_t p[4];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)max;
		p[3] = (dv_param_t)min;
		callout_reporterror(dv_sid_addosekcounter, dv_e_limit, 4, p);
		return -1;
	}

	if ( (c < 0) || (c >= dv_ncounter) )
	{
		dv_param_t p[4];
		p[0] = (dv_param_t)(dv_address_t)name;
		p[1] = (dv_param_t)c;
		p[2] = (dv_param_t)max;
		p[3] = (dv_param_t)min;
		callout_reporterror(dv_sid_addosekcounter, dv_e_id, 4, p);
		return -1;
	}

	dv_id_t id = dv_nosekcounter++;

	dv_osekcounter[id].name = name;
	dv_osekcounter[id].maxvalue = min;
	dv_osekcounter[id].mincycle = max;
	dv_osekcounter[id].counter = c;

	return id;
}

/* dv_addosekalarm() - add an OSEK alarm to the configuration
 *
 * Returns alarm id if successful.
 * On error, returns the negated error id.
 * Error reporting is done by the caller.
*/
static dv_id_t dv_addosekalarm(const char *name, dv_id_t c, dv_u64_t (*af)(dv_id_t a, dv_param_t d))
{
	if ( (c < 0) || (c >= dv_nosekcounter) )
	{
		return -dv_e_id;
	}

	if ( dv_nosekalarm >= dv_maxosekalarm )
	{
		return -dv_e_limit;
	}

	dv_id_t alarm_id = dv_addalarm(name, af, dv_nosekalarm);

	if ( alarm_id < 0 )
	{
		return -dv_e_reported;
	}

	dv_id_t id = dv_nosekalarm++;

	dv_osekalarm[id].name = name;
	dv_osekalarm[id].osekcounter = c;

	return id;
}

static dv_u64_t dv_osekaf_activatetask(dv_id_t a, dv_param_t d)
{
	(void)dv_activatetask(dv_osekalarm[d].obj);

	return dv_osekalarm[d].cycle;
}

static dv_u64_t dv_osekaf_setevent(dv_id_t a, dv_param_t d)
{
	(void)dv_setevent(dv_osekalarm[d].obj, dv_osekalarm[d].more.e);

	return dv_osekalarm[d].cycle;
}

static dv_u64_t dv_osekaf_callacb(dv_id_t a, dv_param_t d)
{
	dv_osekalarm[d].more.acb();

	return dv_osekalarm[d].cycle;
}

static dv_u64_t dv_osekaf_incrementcounter(dv_id_t a, dv_param_t d)
{
	(void)dv_advancecounter(dv_osekalarm[d].obj, 1);

	return dv_osekalarm[d].cycle;
}
