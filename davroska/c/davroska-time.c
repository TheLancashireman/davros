/* davroska-time.c - time-triggereing (counters, alarms etc.) for davroska
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <davroska-inline.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif

/* In davroska, a counter is a 64-bit variable that can be advanced by any amount at any time.
 * Assumption: the counter never needs to overflow.
 *
 * The usual use-cases are:
 *		- advance by 1 on every "tick" of a periodic interval timer
 *		- advance by n ticks of a hardware counter whenever a (possibly variable) delay has elapsed
 *		- advance by 1 on every occurrence of some event (e.g. a message on the network)
 *
 * An alarm is an object that is associated with a counter. It can be set to expire when the counter
 * reaches a predetermined value. Optionally, it can be set to repeat cyclically.
 * When an alarm expires, it calls a function known as the alarm callout function. In this function,
 * you can wake up tasks, send events to tasks, increment other counters, or do anything else that's allowed.
 *
 * API functions:
 *	- dv_advancecounter(c, n) advances the counter c by n ticks and expires any alarms along the way
 *	- dv_readcounter(c) returns the current value of the counter c
 *	- dv_setalarm_abs(c, a, v) sets alarm a to expire when counter c reaches the value v
 *	- dv_setalarm_rel(c, a, v) sets alarm a to expire at v ticks after the current counter value
 *	- dv_getexpirytime(a) gets the expiry time of an alarm
 *
 * Configuration API:
 *	- dv_addcounter
 *	- dv_addalarm
*/

const dv_qty_t dv_maxcounter = DV_CFG_MAXCOUNTER;
const dv_qty_t dv_maxalarm = DV_CFG_MAXALARM;

dv_qty_t dv_ncounter;
dv_qty_t dv_nalarm;

dv_counter_t dv_counter[DV_CFG_MAXCOUNTER];
dv_alarm_t dv_alarm[DV_CFG_MAXALARM];

/* dv_getexpirytime() - returns the expiry time of a given alarm
 *
 * A return value of 0 means that the parameter is out of range or the alarm is not in use
*/
dv_u64_t dv_getexpirytime(dv_id_t a)
{
	if ( (a < 0) || (a >= dv_nalarm) )
	{
		/* Alarm out of range
		*/
		dv_param_t p = (dv_param_t)a;
		(void)callout_reporterror(dv_sid_getexpirytime, dv_e_id, 1, &p);
		return 0;
	}

	return dv_alarm[a].expirytime;
}

/* dv_getcountervalue() - returns the current value of a given counter
 *
 * A return value of 0 means that the parameter is out of range or the alarm is not in use
*/
dv_u64_t dv_getcountervalue(dv_id_t c)
{
	if ( (c < 0) || (c >= dv_ncounter) )
	{
		/* Counter out of range
		*/
		dv_param_t p = (dv_param_t)c;
		(void)callout_reporterror(dv_sid_getcountervalue, dv_e_id, 1, &p);
		return 0;
	}

	return dv_counter[c].value;
}

/* dv_advancecounter() - advances a counter
 *
 * dv_advancecounter(c, n) advances the counter c by n ticks. Any alarms attached to the counter whose
 * 		expiry time is less than the new value are "expired".
*/
dv_statustype_t dv_advancecounter(dv_id_t c, dv_u64_t n)
{
	if ( (c < 0) || (c >= dv_ncounter) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)n;
		return callout_reporterror(dv_sid_advancecounter, dv_e_id, 2, p);
	}

	dv_intstatus_t is = dv_disable();

	dv_u64_t v = dv_counter[c].value;
	dv_counter[c].value += n;
	if ( dv_counter[c].value < v )			/* Overflow */
		dv_panic(dv_panic_CounterOverflow, dv_sid_advancecounter, "counter has overflowed");

	dv_prio_t p = dv_raiseprio();
	dv_id_t a = dv_counter[c].head;
	while ( (a >= 0) &&
			(dv_alarm[a].expirytime <= dv_counter[c].value) )
	{
		dv_counter[c].head = dv_alarm[a].nextalarm;
		dv_u64_t inc = dv_alarm[a].expiryfunction(a,  dv_alarm[a].expiryfunctiondata);
		if ( (dv_alarm[a].expirytime + inc) <= dv_alarm[a].expirytime )
			dv_alarm[a].expirytime = 0;		/* Occurs when inc is 0 and when addition overflows */
		else
			dv_setalarm(c, a, dv_alarm[a].expirytime + inc);
		a = dv_counter[c].head;
	}

	dv_lowerprio(p);
	dv_runqueued_onkernelstack(dv_maxprio-1, p, is);
	dv_restore(is);

	return dv_e_ok;
}

/* dv_setalarm_abs() - set an alarm at an absolute counter value
*/
dv_statustype_t dv_setalarm_abs(dv_id_t c, dv_id_t a, dv_u64_t v)
{
	if ( (c < 0) || (c >= dv_ncounter) || (a < 0) || (a >= dv_nalarm) )
	{
		/* Counter or alarm out of range
		*/
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_abs, dv_e_id, 3, p);
	}

	if ( v == 0 )
	{
		/* An expiry time of 0 means the alarm is not in use
		*/
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_abs, dv_e_value, 3, p);
	}

	if ( dv_alarm[a].expirytime != 0 )
	{
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_abs, dv_e_state, 3, p);
	}

	dv_setalarm(c, a, v);

	return dv_e_ok;
}

/* dv_setalarm_rel() - set an alarm at a counter value relative to the current value
*/
dv_statustype_t dv_setalarm_rel(dv_id_t c, dv_id_t a, dv_u64_t v)
{
	if ( (c < 0) || (c >= dv_ncounter) || (a < 0) || (a >= dv_nalarm) )
	{
		/* Counter or alarm out of range
		*/
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_rel, dv_e_id, 3, p);
	}

	if ( v == 0 )
	{
		/* An expiry time of 0 means the alarm is not in use. We want to be able to set an
		 * alarm when the counter is still zero, so a relative time of 0 is not allowed
		*/
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_rel, dv_e_value, 3, p);
	}

	if ( dv_alarm[a].expirytime != 0 )
	{
		/* Alarm is already in use
		*/
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_rel, dv_e_state, 3, p);
	}

	dv_intstatus_t is = dv_disable();

	dv_u64_t absval = dv_counter[c].value + v;

	if ( absval < dv_counter[c].value )
	{
		/* Calculation has overflowed
		*/
		dv_restore(is);
		dv_param_t p[3];
		p[0] = (dv_param_t)c;
		p[1] = (dv_param_t)a;
		p[2] = (dv_param_t)v;
		return callout_reporterror(dv_sid_setalarm_rel, dv_e_value, 3, p);
	}

	dv_setalarm(c, a, absval);

	dv_restore(is);
	return dv_e_ok;
}

/* dv_addcounter() - add a counter to the set of counters
*/
dv_id_t dv_addcounter(const char *name)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addcounters) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addcounter, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( dv_ncounter >= dv_maxcounter )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addcounter, dv_e_limit, 1, &p);
		return -1;
	}

	dv_id_t id = dv_ncounter++;

	dv_counter[id].name = name;
	dv_counter[id].value = 0;
	dv_counter[id].head = -1;

	return id;
}

/* dv_addalarm() - add an alarm to the set of alarms
*/
dv_id_t dv_addalarm(const char *name, dv_u64_t (*expiryfn)(dv_id_t a, dv_param_t d), dv_u64_t d)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addalarms) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addalarm, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( dv_nalarm >= dv_maxalarm )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addalarm, dv_e_limit, 1, &p);
		return -1;
	}

	dv_id_t id = dv_nalarm++;

	dv_alarm[id].name = name;
	dv_alarm[id].expirytime = 0;
	dv_alarm[id].expiryfunction = expiryfn;
	dv_alarm[id].expiryfunctiondata = d;
	dv_alarm[id].counter = -1;
	dv_alarm[id].nextalarm = -1;

	return id;
}

/* dv_setalarm() - set an alarm at an absolute counter value (internal; no range or state check)
 *
 * If the counter has already reached the counter value, the alarm expires the next time the counter advances.
*/
void dv_setalarm(dv_id_t c, dv_id_t a, dv_u64_t v)
{
	dv_intstatus_t is = dv_disable();

	dv_alarm[a].expirytime = v;
	dv_alarm[a].counter = c;

	dv_id_t pred = dv_counter[c].head;

	if ( pred < 0 )
	{
		/* Alarm list is empty. Make new alarm the head and return.
		*/
		dv_alarm[a].nextalarm = -1;
		dv_counter[c].head = a;

		dv_restore(is);
		return;
	}

	if ( dv_alarm[pred].expirytime > v )
	{
		/* First alarm comes after new alarm. Insert new alarm at head and return
		*/
		dv_alarm[a].nextalarm = dv_counter[c].head;
		dv_counter[c].head = a;

		dv_restore(is);
		return;
	}

	while ( dv_alarm[pred].nextalarm >= 0 )
	{
		if ( dv_alarm[dv_alarm[pred].nextalarm].expirytime > v )
		{
			/* Found a place to insert the alarm. Insert it and return
			*/
			dv_alarm[a].nextalarm = dv_alarm[pred].nextalarm;
			dv_alarm[pred].nextalarm = a;

			dv_restore(is);
			return;
		}

		pred = dv_alarm[pred].nextalarm;
	}

	/* Alarm gets placed at end of queue
	*/
	dv_alarm[a].nextalarm = -1;
	dv_alarm[pred].nextalarm = a;

	dv_restore(is);
}
