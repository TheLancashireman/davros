/*	davroska-extended.c -  davroska extended task (event) API
 *
 *	Copyright David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#define DV_ASM  0
#include <dv-config.h>
#include <davroska.h>
#include <davroska-inline.h>

const dv_qty_t dv_maxextended = DV_CFG_MAXEXTENDED;

#if DV_CFG_MAXEXTENDED > 0

dv_extended_t dv_extended[DV_CFG_MAXEXTENDED];

dv_qty_t dv_nextended;
dv_stackword_t *dv_extstacks;

dv_boolean_t dv_onkernelstack;
dv_jmpbuf_t *dv_kjmpbuf;

/* dv_waitevent() - wait for one or more events
*/
dv_statustype_t dv_waitevent(dv_eventmask_t evts)
{
	if ( !dv_extendedtask(dv_currentexe) )
	{
		dv_param_t p = (dv_param_t)evts;
		return callout_reporterror(dv_sid_waitevent, dv_e_access, 1, &p);
	}

	if ( dv_exe[dv_currentexe].mutexlist >= 0 )
	{
		dv_param_t p = (dv_param_t)evts;
		return callout_reporterror(dv_sid_waitevent, dv_e_resource, 1, &p);
	}

	/* We're running on a private stack here!
	*/
	dv_intstatus_t is = dv_disable();

	dv_id_t ext = dv_exe[dv_currentexe].extended;

	if ( (evts & dv_extended[ext].events_pending) != 0 )
	{
		/* One or more events already pending - just return
		 * It would be possible to have this branch requeue the task here to permit equal-priority
		 * tasks to run. The OSEK spec doesn't say.
		 * However, it would complicate the scheduler somewhat, so we'll take the easy way out.
		*/
		dv_restore(is);
		return dv_e_ok;
	}

	dv_extended[ext].events_awaited = evts;
	dv_exe[dv_currentexe].state = dv_waiting;

	dv_jmpbuf_t jb;		/* Stores the current state for use when released */
	dv_extended[ext].jb = &jb;

	if ( dv_setjmp(jb) == 0 )	/* Task release causes non-zero return here */
	{
		dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

		dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_waitevent, "dv_longjmp() returned to caller");
	}

	dv_extended[ext].jb = DV_NULL;
	dv_extended[ext].events_awaited = 0;
	dv_restore(is);
	return dv_e_ok;
}

/* dv_setevent() - set one or more events for an extended task
*/
dv_statustype_t dv_setevent(dv_id_t t, dv_eventmask_t evts)
{
	if ( (t < 1) || (t >= dv_ntask) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)evts;
		return callout_reporterror(dv_sid_setevent, dv_e_id, 2, p);
	}

	if ( !dv_extendedtask(t) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)evts;
		return callout_reporterror(dv_sid_setevent, dv_e_access, 2, p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_exe[t].state == dv_suspended )
	{
		dv_restore(is);
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)evts;
		return callout_reporterror(dv_sid_setevent, dv_e_state, 2, p);
	}

#if DV_DEBUG
	dv_printf("dv_setevent: events 0x%x%08x\n", (dv_u32_t)(evts>>32), (dv_u32_t)evts);
#endif

	dv_id_t ext = dv_exe[t].extended;

	dv_extended[ext].events_pending |= evts;

	if ( (dv_exe[t].state == dv_waiting) &&
		  ((dv_extended[ext].events_pending & dv_extended[ext].events_awaited) != 0) )
	{
		/* Release the executable
		*/
#if DV_DEBUG
		dv_printf("dv_setevent: releasing %s\n", dv_exe[t].name);
#endif
		dv_exe[t].state = dv_ready;
		dv_enqueue(dv_exe[t].baseprio, t);

		/* Now run any executables that are of higher priority than caller.
		*/
		dv_runqueued_onkernelstack(dv_exe[t].baseprio, dv_exe[dv_currentexe].currprio, is);
	}

	dv_restore(is);
	return dv_e_ok;
}

/* dv_getevent() - obtain pending events for a task
*/
dv_statustype_t dv_getevent(dv_id_t t, dv_eventmask_t *evts)
{
	if ( (t < 1) || (t >= dv_ntask) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)(dv_address_t)evts;
		return callout_reporterror(dv_sid_getevent, dv_e_id, 2, p);
	}

	if ( !dv_extendedtask(t) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)(dv_address_t)evts;
		return callout_reporterror(dv_sid_getevent, dv_e_access, 2, p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_exe[t].state == dv_suspended )
	{
		dv_restore(is);
		dv_param_t p[2];
		p[0] = (dv_param_t)t;
		p[1] = (dv_param_t)(dv_address_t)evts;
		return callout_reporterror(dv_sid_getevent, dv_e_state, 2, p);
	}

	*evts = dv_extended[dv_exe[t].extended].events_pending;

	dv_restore(is);
	return dv_e_ok;
}

/* dv_clearevent() - clear pending events
*/
dv_statustype_t dv_clearevent(dv_eventmask_t evts)
{
	if ( !dv_extendedtask(dv_currentexe) )
	{
		dv_param_t p = (dv_param_t)evts;
		return callout_reporterror(dv_sid_clearevent, dv_e_access, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	dv_extended[dv_exe[dv_currentexe].extended].events_pending &= ~evts;

	dv_restore(is);
	return dv_e_ok;
}

/* dv_addextendedtask() - add an extended task to the system
 *
 * Add a normal task with max. activations = 1, then allocate stack for it
 *
 * ToDo: add check for free stack space.
*/
dv_id_t dv_addextendedtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_u32_t stackbytes)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addtasks) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addextendedtask, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( dv_nextended >= dv_maxextended )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addextendedtask, dv_e_limit, 1, &p);
		return -1;
	}

	if ( stackbytes <= 0 )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addextendedtask, dv_e_value, 1, &p);
		return -1;
	}

	dv_id_t id = dv_addtask(name, fn, prio, 1);

	if ( id < 0 )
		return id;

	dv_id_t n = dv_nextended++;
	dv_u32_t l = dv_stackround(stackbytes);

	dv_exe[id].extended = n;

	dv_extended[n].stackbase = dv_extstacks;
	dv_extended[n].stacklen = l;

	dv_extstacks = &dv_extstacks[(l + DV_CANARY)/sizeof(dv_stackword_t)];
	dv_extended[n].initialsp = dv_extstacks;

	return id;
}

/* dv_extended_init() - initialise the extended task support
*/
void dv_extended_init(dv_u32_t *stackbase)
{
	/* Align the stackbase provided to a canary boundary
	*/
	dv_extstacks = (dv_stackword_t *)dv_stackround((dv_address_t)stackbase);

	/* We should be on the kernel stack at the moment
	*/
	dv_onkernelstack = 1;
}

/* dv_runextended() - switch stack and run executable
*/
void dv_runextended(dv_id_t e, dv_intstatus_t is)
{
	dv_jmpbuf_t jb;
	dv_request_t *req = (dv_request_t *)dv_setjmp(jb);

	if ( req == 0 )		/* Returns non-zero if there's some nested activity to do */
	{
		dv_onkernelstack = 0;
		dv_kjmpbuf = &jb;

		if ( dv_extended[dv_exe[e].extended].jb == DV_NULL )
		{
			/* A new activation of an extended task. Essentially, a directed longjump
			*/
			dv_switchcall(e, is, dv_extended[dv_exe[e].extended].initialsp, &dv_startextendedtask);
			dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_scheduler, "dv_switchcall() returned to caller");
		}
		else
		{
			/* Resume extended task after a preemption or wait.
			*/
			dv_longjmp(*dv_extended[dv_exe[e].extended].jb, dv_e_longjmp_ok);
			dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_scheduler, "dv_longjmp() returned to caller");
		}
	}
	else
	{
		/* An API or interrupt during an extended task switched back to the kernel stack.
		*/
		dv_onkernelstack = 1;
		switch ( req->code )
		{
		case dv_req_rq:
			dv_runqueued(req->p.rq.high, req->p.rq.low, req->p.rq.is);
			break;
		default:
			dv_panic(dv_panic_UnknownRequest, dv_sid_scheduler, "unknown kernel-stack request received");
			break;
		}
		dv_onkernelstack = 0;
		dv_longjmp(*(req->return_jb), 1);
	}
}

/* dv_startextendedtask() - wrapper for an extended task
*/
void dv_startextendedtask(dv_id_t e, dv_intstatus_t is)
{
	dv_restore(is);

	(*dv_exe[e].func)();

	(void)dv_disable();

	dv_longjmp(*dv_exe[e].jb, 1);

	dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_scheduler, "dv_longjmp() returned to caller");
}

/* dv_runqueued_onkernelstack() - call dv_runqueued, after switching to kernel stack if necessary
*/
void dv_runqueued_onkernelstack(dv_prio_t high, dv_prio_t low, dv_intstatus_t is)
{
	if ( dv_onkernelstack )
		 dv_runqueued(high, low, is);
	else
	{
		dv_jmpbuf_t jb;
		dv_request_t req;
		if ( dv_setjmp(jb) == 0 )
		{
			req.code = dv_req_rq;
			req.return_jb = &jb;
			req.p.rq.high = high;
			req.p.rq.low = low;
			req.p.rq.is = is;

			if ( dv_kjmpbuf == DV_NULL )
				dv_panic(dv_panic_NoKernelAnchor, dv_sid_scheduler, "kernel request failed; no anchor");
			dv_longjmp(*dv_kjmpbuf, (int)&req);
		}
	}
}

#endif
