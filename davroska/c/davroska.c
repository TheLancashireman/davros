/* davroska.c - OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER

#include <davroska-inline.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif

#define DV_NQUEUE	(DV_CFG_MAXPRIO+2)			/* Add extra queues for idle and kernel */
#define DV_NEXE		(DV_CFG_MAXEXE+1)			/* Add an extra executable for the idle loop */
#define DV_NSLOT	(DV_NQUEUE*2 + DV_NEXE + DV_CFG_NSLOT_EXTRA)

const dv_qty_t dv_maxexe = DV_NEXE;
const dv_qty_t dv_maxprio = DV_CFG_MAXPRIO;
const dv_qty_t dv_maxslot = DV_NSLOT+1;
const dv_qty_t dv_maxmutex = DV_CFG_MAXMUTEX;

dv_configstate_t *dv_configstate;		/* Current state of configuration sequence */

dv_prio_t dv_highprio;					/* Priority of highest activated exe (before running) */
dv_prio_t dv_highestprio;				/* Highest priority in use */
dv_id_t dv_currentexe;					/* Current executable */

dv_qty_t dv_nexe;						/* No. of executables created */
dv_qty_t dv_ntask;						/* No. of tasks created */
dv_qty_t dv_nisr;						/* No. of ISRs created */
dv_qty_t dv_nmutex;						/* No. of mutexes created */

dv_prio_t dv_maxtaskprio;				/* Highest-priority task */

dv_exe_t dv_exe[DV_NEXE];				/* Executables */
dv_q_t dv_queue[DV_NQUEUE];				/* Priority queues (one per priority) */
dv_id_t dv_slots[DV_NSLOT];				/* Ring buffers for the queues */
dv_mutex_t dv_mutex[DV_CFG_MAXMUTEX];	/* Mutexes */

dv_softvector_t dv_vectors[DV_NVECTOR];

/* Forward function declaractions
*/
static void dv_idle(void);
static void dv_createqueues(void);
static void dv_calculatelevels(void);
static dv_statustype_t dv_activateexe(dv_id_t e);
static dv_statustype_t dv_activateexe2(dv_id_t ei, dv_intstatus_t is);
static dv_id_t dv_addexe(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);

/* Interrupt vectoring
*/
static int dv_isvectorfree(dv_id_t vec);
static void dv_setvector(dv_id_t vec, dv_statustype_t (*fn)(dv_id_t p), dv_id_t p);
static void dv_initvectors(void);

static void dv_addtasks(dv_id_t mode);
static void dv_addisrs(dv_id_t mode);
static void dv_addgroups(dv_id_t mode);
static void dv_addmutexes(dv_id_t mode);
static void dv_addcounters(dv_id_t mode);
static void dv_addalarms(dv_id_t mode);

/* ===================================================================================================================
 * Runtime API functions
 * ===================================================================================================================
*/

/* dv_activatetask() - activate a task
 *
 * If it's a task, activate it. Otherwise: error!
*/
dv_statustype_t dv_activatetask(dv_id_t t)
{
	if ( (t < 1) || (t >= dv_ntask) )
	{
		dv_param_t p = (dv_param_t)t;
		return callout_reporterror(dv_sid_activatetask, dv_e_id, 1, &p);
	}

	return dv_activateexe(t);
}

/* dv_chaintask() - terminate the called and activate a task
 *
 * If it's a task, terminate me then activate it.
*/
dv_statustype_t dv_chaintask(dv_id_t t)
{
	if ( (t < 1) || (t >= dv_ntask) )
	{
		dv_param_t p = (dv_param_t)t;
		return callout_reporterror(dv_sid_chaintask, dv_e_id, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( (t != dv_currentexe) && (dv_exe[t].nact >= dv_exe[t].maxact) )
	{
		dv_restore(is);
		dv_param_t p = (dv_param_t)t;
		return callout_reporterror(dv_sid_chaintask, dv_e_limit, 1, &p);
	}

	/* Sanity checks
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_CurrentExeCorrupt, dv_sid_chaintask, "dv_currentexe is out of range");

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_CurrentExeDead, dv_sid_chaintask, "current executable has no termination jmp_buf");

	(void)dv_activateexe2(t, is);

	dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

	dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_chaintask, "dv_longjmp() returned to caller");
}

/* dv_terminatetask() - terminate the calling task
*/
dv_statustype_t dv_terminatetask(void)
{
	(void)dv_disable();

	/* Sanity checks
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_CurrentExeCorrupt, dv_sid_terminatetask, "dv_currentexe is out of range");

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_CurrentExeDead, dv_sid_terminatetask, "current executable has no termination jmp_buf");

	dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

	dv_panic(dv_panic_ReturnFromLongjmp, dv_sid_terminatetask, "dv_longjmp() returned to caller");
}

/* dv_takemutex() - take a mutex
 *
 * The mutex's ceiling priority must be higher than the caller's base priority
*/
dv_statustype_t dv_takemutex(dv_id_t mutex)
{
	if ( (mutex < 0) || (mutex >= dv_nmutex) )
	{
		dv_param_t p = (dv_param_t)mutex;
		return callout_reporterror(dv_sid_takemutex, dv_e_id, 1, &p);
	}

	if ( dv_mutex[mutex].ceiling < dv_exe[dv_currentexe].baseprio )
	{
		dv_param_t p = (dv_param_t)mutex;
		return callout_reporterror(dv_sid_takemutex, dv_e_access, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_mutex[mutex].owner == dv_currentexe )
	{
		if ( dv_mutex[mutex].ntake >= dv_mutex[mutex].maxtake )
		{
			(void)dv_restore(is);
			dv_param_t p = (dv_param_t)mutex;
			return callout_reporterror(dv_sid_takemutex, dv_e_nesting, 1, &p);
		}
		else
		{
			dv_mutex[mutex].ntake++;
			(void)dv_restore(is);
			return dv_e_ok;
		}
	}

	/* The OSEK specification says that this should be an error. However, because of the ceiling priority
	 * check (above), this error indicates a fault in the scheduler
	*/
	if ( dv_mutex[mutex].owner >= 0 )			/* Indicates that an exe of lower priority than the owner is running */
		dv_panic(dv_panic_MutexOccupied, dv_sid_takemutex, "mutex is occupied");

	/* Mark mutex as owned by current executable
	*/
	dv_mutex[mutex].owner = dv_currentexe;
	dv_mutex[mutex].ntake = 1;

	/* Insert the mutex at the head of the current executable's list
	*/
	dv_mutex[mutex].next = dv_exe[dv_currentexe].mutexlist;
	dv_exe[dv_currentexe].mutexlist = mutex;

	/* Save the current priority in the mutex and increase the executable's priority to the ceiling.
	 * Put executable into ceiling queue if the priority actually increases
	*/
	dv_mutex[mutex].saveprio = dv_exe[dv_currentexe].currprio;
	if ( dv_exe[dv_currentexe].currprio < dv_mutex[mutex].ceiling )
	{
		dv_prio_t p = dv_exe[dv_currentexe].currprio = dv_mutex[mutex].ceiling;
		dv_enqueue(p, dv_currentexe);
#if DV_DEBUG
		dv_printf("dv_takemutex() - set IRQ level %d\n", dv_queue[p].level);
#endif
		dv_setirqlevel(dv_queue[p].level);
	}

	(void)dv_restore(is);
	return dv_e_ok;
}

/* dv_dropmutex() - drop a mutex
 *
 * The mutex must have been taken by the executable, and if it is the last drop (ntake will become zero)
 * the mutex must be the most recently taken mutex (i.e. at the head of the exe's mutex list)
*/
dv_statustype_t dv_dropmutex(dv_id_t mutex)
{
	if ( (mutex < 0) || (mutex >= dv_nmutex) )
	{
		dv_param_t p = (dv_param_t)mutex;
		return callout_reporterror(dv_sid_dropmutex, dv_e_id, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_mutex[mutex].owner != dv_currentexe )
	{
		(void)dv_restore(is);
		dv_param_t p = (dv_param_t)mutex;
		return callout_reporterror(dv_sid_dropmutex, dv_e_nofunc, 1, &p);
	}

	if ( dv_mutex[mutex].ntake > 1 )
	{
		dv_mutex[mutex].ntake--;
		dv_restore(is);
		return dv_e_ok;
	}

	dv_mutex[mutex].ntake = 0;
	dv_mutex[mutex].owner = -1;
	dv_exe[dv_currentexe].mutexlist = dv_mutex[mutex].next;
	dv_mutex[mutex].next = -1;

	dv_prio_t high = dv_mutex[mutex].ceiling;
	dv_prio_t low = dv_exe[dv_currentexe].currprio = dv_mutex[mutex].saveprio;

	if ( low < high )
	{
		if ( dv_dequeue(high) != dv_currentexe )
			dv_panic(dv_panic_QueueCorrupt, dv_sid_dropmutex, "mutex owner is not at head of ceiling priority queue");

		dv_runqueued_onkernelstack(high, low, is);
	}
	
	(void)dv_restore(is);
	return dv_e_ok;
}

/* dv_startos() - start the OS
*/
dv_statustype_t dv_startos(dv_id_t mode)
{
	(void)dv_disable();

	/* Initialise the priority queues
	*/
	for (int i = 0; i <= dv_maxprio; i++ )
	{
		dv_queue[i].prio = i;		/* Set the priority */
		dv_queue[i].nslots = 2;		/* Reserve an entry for a raised priority and an entry for the gap */
	}

	/* Initialise the interrupt systems
	*/
	dv_initvectors();
	dv_init_interrupt_controller();

	/* Initialise extended task handling
	*/
	extern dv_u32_t dv_end_bss;
	dv_extended_init(&dv_end_bss);

	/* Ensure that no executables can preempt until everything is initialised
	*/
#if DV_DEBUG
	dv_printf("dv_startos() - set IRQ level %d\n", 8);
#endif
	dv_setirqlevel(DV_LOCKALL_LEVEL);

	/* Create the idle loop executable
	*/
	dv_id_t idle = dv_addexe("idle-loop", &dv_idle, 0, 1);

	if ( idle < 0 )
		dv_panic(dv_panic_IdleCreation, dv_sid_startos, "could not create the idle executable");

	dv_ntask = 1;	/* Range check for tasks is from 1 to dv_ntask */

	dv_addtasks(mode);
	dv_addisrs(mode);

	dv_createqueues();
	dv_calculatelevels();

	dv_addgroups(mode);

	dv_addmutexes(mode);
	dv_addcounters(mode);
	dv_addalarms(mode);

	/* Now activate the idle loop.
	 * Can't use dv_activatexe here because the idle loop must not run!
	 * Afterwards, increase the priority to maximum so that autostart tasks cannot run.
	*/
	dv_exe[idle].nact++;
	dv_enqueue(dv_exe[idle].currprio, idle);
	dv_currentexe = idle;
	dv_exe[idle].state = dv_running;

	(void)dv_raiseprio();
	callout_autostart(mode);
	callout_startup();
	dv_lowerprio(dv_exe[idle].baseprio);

	dv_runqueued_onkernelstack(dv_maxprio-1, dv_exe[idle].baseprio, DV_INTENABLED);

	/* If the task - or the other tasks that it starts - ever die,
	 * we enable interrupts and drop into the idle loop waiting for an interrupt.
	*/
#if DV_DEBUG
	dv_printf("dv_startos() - set IRQ level %d\n", 0);
#endif
	dv_setirqlevel(0);
	dv_restore(DV_INTENABLED);
	dv_idle();

	/* To keep the compiler happy
	*/
	return dv_e_ok;
}

/* Wrappers for callout_addxxx
 *
 * These wrapper functions ensure that the config state data uses temporary stack
 * so that the only global data used is the pointer to the current cfgstate variable.
 *
 * This might seem like over-complicated for now, but when dv_addinternalmutexes() gets implemented
 * we'll need to maintain an array of all executables that use the mutexes so that we can calculate the
 * running priority and configure all of them.
*/
/* dv_addtasks() - wrapper for callout_addtasks
*/
static void dv_addtasks(dv_id_t mode)
{
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addtasks;
	cfgstate.data = DV_NULL;
	dv_configstate = &cfgstate;

	callout_addtasks(mode);

	dv_configstate = DV_NULL;
}

/* dv_addgroups() - wrapper for callout_addgroups
*/
typedef struct dv_groupdef_s
{
	const char *name;
	dv_boolean_t no_preempt;
	dv_u8_t members[DV_NEXE];
} dv_groupdef_t;

static void dv_addgroups(dv_id_t mode)
{
	dv_groupdef_t groupdef;
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addgroups;
	cfgstate.data = &groupdef;
	dv_configstate = &cfgstate;

	groupdef.name = DV_NULL;

	callout_addgroups(mode);

	dv_configstate = DV_NULL;

	if ( groupdef.name != DV_NULL )
	{
		/* The addgroups callout left an unfinished group
		*/
		callout_reporterror(dv_sid_startos, dv_e_state, 0, DV_NULL);
	}
}

/* dv_addisrs() - wrapper for callout_addisrs
*/
static void dv_addisrs(dv_id_t mode)
{
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addisrs;
	cfgstate.data = DV_NULL;
	dv_configstate = &cfgstate;

	callout_addisrs(mode);

	dv_configstate = DV_NULL;
}

/* dv_addmutexes() - wrapper for callout_addmutexes
*/
static void dv_addmutexes(dv_id_t mode)
{
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addmutexes;
	cfgstate.data = DV_NULL;
	dv_configstate = &cfgstate;

	callout_addmutexes(mode);

	dv_configstate = DV_NULL;
}

/* dv_addcounters() - wrapper for callout_addcounters
*/
static void dv_addcounters(dv_id_t mode)
{
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addcounters;
	cfgstate.data = DV_NULL;
	dv_configstate = &cfgstate;

	callout_addcounters(mode);

	dv_configstate = DV_NULL;
}

/* dv_addalarms() - wrapper for callout_addalarms
*/
static void dv_addalarms(dv_id_t mode)
{
	dv_configstate_t cfgstate;
	cfgstate.phase = ph_addalarms;
	cfgstate.data = DV_NULL;
	dv_configstate = &cfgstate;

	callout_addalarms(mode);

	dv_configstate = DV_NULL;
}

/* dv_shutdown() - shut down the OS
*/
void dv_shutdown(dv_statustype_t reason)
{
	(void)dv_disable();
	callout_shutdown(reason);
	for (;;) {}
}

/* ===================================================================================================================
 * Configuration API functions
 * ===================================================================================================================
*/

/* dv_addtask() - add a task to the set of executables
*/
dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addtasks) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addtask, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( maxact < 1 )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addtask, dv_w_value, 1, &p);
		maxact = 1;
	}

	if ( prio < 1 )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addtask, dv_e_value, 1, &p);
		return -1;
	}

	dv_id_t id = dv_addexe(name, fn, prio, maxact);

	if ( id >= 0 )
	{
		dv_ntask++;
		if ( prio > dv_maxtaskprio )
			dv_maxtaskprio = prio;
	}

	return id;
}

/* dv_addisr() - add an isr to the set of executables
*/
dv_id_t dv_addisr(const char *name, void (*fn)(void), dv_id_t irqid, dv_prio_t prio)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addisrs) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addisr, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( prio <= dv_maxtaskprio )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addisr, dv_e_value, 1, &p);
		return -1;
	}

	if ( !dv_isvectorfree(irqid) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addisr, dv_e_state, 1, &p);
		return -1;
	}

	dv_id_t id = dv_addexe(name, fn, prio, 1);

	if ( id >= 0 )
	{
		dv_nisr++;
		dv_exe[id].irqid = irqid;
#if DV_DEBUG
		dv_printf("dv_addisr() = added %s id %d baseprio %d runprio %d irqid %d\n",
			dv_exe[id].name, id, dv_exe[id].baseprio, dv_exe[id].runprio, dv_exe[id].irqid);
#endif
		dv_setvector(irqid, dv_activateexe, id);
	}

	return id;
}

/* dv_startgroup() - start configuring a group of executables
*/
void dv_startgroup(const char *name, dv_boolean_t non_preemptable)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addgroups) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_startgroup, dv_e_calllevel, 1, &p);
		return;
	}

	dv_groupdef_t *g = (dv_groupdef_t *)dv_configstate->data;

	if ( g->name != DV_NULL )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_startgroup, dv_e_state, 1, &p);
		return;
	}

	g->name = name;
	g->no_preempt = non_preemptable;

	for ( int i = 0; i < DV_NEXE; i++ )
		g->members[i] = 0;
}

/* dv_addtogroup() - add an executable to a group
*/
void dv_addtogroup(dv_id_t e)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addgroups) )
	{
		dv_param_t p = (dv_param_t)e;
		callout_reporterror(dv_sid_addtogroup, dv_e_calllevel, 1, &p);
		return;
	}

	dv_groupdef_t *g = (dv_groupdef_t *)dv_configstate->data;

	if ( g->name == DV_NULL )
	{
		dv_param_t p = (dv_param_t)e;
		callout_reporterror(dv_sid_addtogroup, dv_e_state, 1, &p);
		return;
	}

	if ( (e < 1) || (e >= dv_nexe) )
	{
		dv_param_t p[2];
		p[0] = (dv_param_t)e;
		p[1] = (dv_param_t)(dv_address_t)g->name;
		callout_reporterror(dv_sid_addtogroup, dv_e_id, 2, p);
		return;
	}

	if ( g->no_preempt && (e >= dv_ntask) )
	{
		/* Only tasks can be made "non-preemptable" in this sense
		*/
		dv_param_t p[2];
		p[0] = (dv_param_t)e;
		p[1] = (dv_param_t)(dv_address_t)g->name;
		callout_reporterror(dv_sid_addtogroup, dv_e_nofunc, 2, p);
		return;
	}

	g->members[e] = 1;
}

/* dv_finishgroup() - finish configuring a group
 *
 * calculate ceiling priority and set the runprio of every executable in the group
*/
void dv_finishgroup(void)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addgroups) )
	{
		callout_reporterror(dv_sid_finishgroup, dv_e_calllevel, 0, DV_NULL);
		return;
	}

	dv_groupdef_t *g = (dv_groupdef_t *)dv_configstate->data;

	if ( g->name == DV_NULL )
	{
		callout_reporterror(dv_sid_finishgroup, dv_e_state, 0, DV_NULL);
		return;
	}

	dv_prio_t gprio = 0;

	if ( g->no_preempt )
	{
		/* For non-preemptable, find highest-priority task
		*/
		for ( int i = 1; i < dv_ntask; i++ )
			if ( gprio < dv_exe[i].baseprio )
				gprio = dv_exe[i].baseprio;
	}
	else
	{
		/* Otherwise, find highest-priority among group (can contain ISRs)
		*/
		for ( int i = 1; i < dv_nexe; i++ )
			if ( g->members[i] && gprio < dv_exe[i].baseprio )
				gprio = dv_exe[i].baseprio;
	}

	/* Set runprio of all members that have a lower runprio
	*/
	for ( int i = 1; i < dv_nexe; i++ )
		if ( g->members[i] && dv_exe[i].runprio < gprio )
			dv_exe[i].runprio = gprio;

	/* Allow configuration of another group
	*/
	g->name = DV_NULL;
}

/* dv_addmutex() - add a mutex to the set of mutexes
 *
 * The ceiling priority is determined later....
*/
dv_id_t dv_addmutex(const char *name, dv_qty_t maxtake)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addmutexes) )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addmutex, dv_e_calllevel, 1, &p);
		return -1;
	}

	if ( dv_nmutex >= dv_maxmutex )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addmutex, dv_e_limit, 1, &p);
		return -1;
	}

	if ( maxtake < 1 )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addmutex, dv_w_value, 1, &p);
		maxtake = 1;
	}

	dv_id_t id = dv_nmutex++;

	dv_mutex[id].name = name;
	dv_mutex[id].ceiling = 0;
	dv_mutex[id].maxtake = maxtake;
	dv_mutex[id].ntake = 0;
	dv_mutex[id].owner = -1;
	dv_mutex[id].saveprio = -1;
	dv_mutex[id].next = -1;

	return id;
}

/* dv_addmutexuser() - add an executable to the notional list of users of a mutex
 *
 * The ceiling priority of the mutex is determined by one or more calls to this function
*/
void dv_addmutexuser(dv_id_t mx, dv_id_t e)
{
	if ( (dv_configstate == DV_NULL) || (dv_configstate->phase != ph_addmutexes) )
	{
		dv_param_t p = (dv_param_t)mx;
		callout_reporterror(dv_sid_addmutexuser, dv_e_calllevel, 1, &p);
		return;
	}

	if ( (mx < 0) || (mx >= dv_nmutex) )
	{
		dv_param_t p = (dv_param_t)mx;
		callout_reporterror(dv_sid_addmutexuser, dv_e_id, 1, &p);
		return;
	}

	if ( (e < 0) || (e >= dv_nexe) )
	{
		dv_param_t p = (dv_param_t)mx;
		callout_reporterror(dv_sid_addmutexuser, dv_e_access, 1, &p);
		return;
	}

	if ( dv_mutex[mx].ceiling < dv_exe[e].baseprio )
		dv_mutex[mx].ceiling = dv_exe[e].baseprio;
}

/* ===================================================================================================================
 * Internal functions
 * ===================================================================================================================
*/

/* dv_addexe() - add an executable to the set of executables
*/
static dv_id_t dv_addexe(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
{
	if ( dv_nexe >= dv_maxexe )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addexe, dv_e_limit, 1, &p);
		return -1;
	}

	if ( prio >= dv_maxprio )
	{
		dv_param_t p = (dv_param_t)(dv_address_t)name;
		callout_reporterror(dv_sid_addexe, dv_e_value, 1, &p);
		return -1;
	}

	dv_id_t id = dv_nexe++;

	dv_exe[id].name = name;
	dv_exe[id].func = fn;
	dv_exe[id].extended = -1;
	dv_exe[id].maxact = maxact;
	dv_exe[id].nact = 0;
	dv_exe[id].baseprio = prio;
	dv_exe[id].runprio = prio;
	dv_exe[id].currprio = prio;
	dv_exe[id].state = dv_suspended;
	dv_exe[id].mutexlist = -1;
	dv_exe[id].irqid = -1;

	dv_queue[prio].nslots += maxact;

	if ( prio > dv_highestprio )
		dv_highestprio = prio;

	return id;
}

/* dv_activateexe() - activate an executable
 *
 * If the task has a higher priority than the caller, call it.
 * Otherwise enqueue it.
*/
static dv_statustype_t dv_activateexe(dv_id_t e)
{
#if DV_DEBUG
	dv_printf("dv_activateexe() - %d\n", e);
#endif
	dv_intstatus_t is = dv_disable();

	/* Max activations check
	*/
	if ( dv_exe[e].nact >= dv_exe[e].maxact )
	{
		(void)dv_restore(is);
		dv_param_t p = (dv_param_t)e;
		return callout_reporterror(dv_sid_activatetask, dv_e_limit, 1, &p);
	}

	/* Do part 2
	*/
	dv_statustype_t s = dv_activateexe2(e, is);
#if DV_DEBUG
	dv_printf("dv_activateexe() : dv_activateexe2() returned %d\n", s);
#endif
	return s;
}

/* dv_runqueued() - run all higher-priority executables
*/
void dv_runqueued(dv_prio_t high, dv_prio_t low, dv_intstatus_t is)
{
	dv_prio_t p = high;
	dv_id_t exe = dv_currentexe;

	while ( p > low )
	{
		dv_id_t e = dv_qhead(p);

		if ( e == 0 )
		{
			/* The idle executable is never in a higher-priority queue
			*/
			dv_panic(dv_panic_QueueCorrupt, dv_sid_scheduler, "idle executable has elevated its priority");
		}
		else
		if ( e > 0 )
		{
			if ( dv_exe[exe].state == dv_running )
			{
				callout_postexe();
				dv_exe[exe].state = dv_ready;
				/* Preempted, so current priority is not reduced
				*/
			}

			/* Run the executable. When it terminates or waits it gets removed from the queue
			*/
			dv_jmpbuf_t jb;
			dv_currentexe = e;

			/* Set the current priority to the base priority here.
			 * Can't do it at activate time because another instance might be running (basic tasks)
			 * It might not be necessary with careful management because the priority is reduced on
			 * termination too, but "belt & braces" ;-)
			*/
			dv_exe[e].currprio = dv_exe[e].baseprio;

			if ( dv_exe[e].currprio < dv_exe[e].runprio )
			{
				dv_exe[e].currprio = dv_exe[e].runprio;
				dv_enqueue(dv_exe[e].currprio, e);
			}
			
			dv_exe[e].state = dv_running;
			dv_exe[e].jb = &jb;

			/* Set the interrupt mutex level
			*/
			dv_prio_t eprio = dv_exe[e].currprio;
			dv_prio_t level = dv_queue[eprio].level;

#if DV_DEBUG
			dv_printf("dv_runqueued() - new exe %d; prio %d, set IRQ level %d\n", e, eprio, level);
#endif
			(void)dv_setirqlevel(level);

			/* Pre-exe hook for incoming exe
			*/
			callout_preexe();

			if ( dv_setjmp(jb) == 0 )	/* TerminateTask(), ChainTask() and WaitEvent() cause non-zero return here */
			{
				if ( dv_extendedtask(e) )
				{
					dv_runextended(e, is);
				}
				else
				{
					/* Interrupt state of caller is preserved for incoming exe
					*/
					dv_restore(is);

					/* Run the exe
					*/
					(*dv_exe[e].func)();
				}
			}

			/* On return or termination don't propagate the interrupt state
			*/
			(void)dv_disable();

			dv_set_onkernelstack();

			callout_postexe();

			/* ToDo: release any mutexes that the executable holds
			 * This needs to be done before dequeueing from the baseprio/runprio queues because
			 * the executable is only in an elevated priority queue if the priority really increases.
			*/

			/* Dequeue the executable (with sanity check)
			*/
			if ( eprio != dv_exe[e].baseprio )
				if ( dv_dequeue(eprio) != e )
					dv_panic(dv_panic_QueueCorrupt, dv_sid_scheduler,
								"terminating executable is not at head of its runprio queue");

			dv_exe[e].currprio = dv_exe[e].baseprio;
			if ( dv_dequeue(dv_exe[e].baseprio) != e )
				dv_panic(dv_panic_QueueCorrupt, dv_sid_scheduler,
							"terminating executable is not at head of its baseprio queue");

			if ( dv_extendedtask(e) &&  dv_taskwaiting(e) )
			{
				dv_exe[e].state = dv_waiting;
			}
			else
			{
				/* Task is terminating
				*/
				dv_exe[e].nact--;
				if ( dv_exe[e].nact <= 0 )
					dv_exe[e].state = dv_suspended;
				else
					dv_exe[e].state = dv_ready;
			}
			dv_exe[e].jb = DV_NULL;
		}
		else
		{
			/* Queue is empty. Drop down to next lower priority.
			*/
			p--;
		}
	}

	/* When all higher-priority activity is done, back to the original caller
	*/
	if ( dv_currentexe != exe )
	{
		/* Executable was preempted while running the queue
		*/
		dv_currentexe = exe;
		dv_prio_t eprio = dv_exe[dv_currentexe].currprio;
		dv_prio_t level = dv_queue[eprio].level;
#if DV_DEBUG
		dv_printf("dv_runqueued() - resume old exe %d; prio %d set IRQ level %d\n", eprio, level);
#endif
		dv_setirqlevel(level);
		dv_exe[dv_currentexe].state = dv_running;
		callout_preexe();
	}
}

/* dv_raiseprio() - increase priority of current executable
 *
 * Kernel functions use this to bump the priority of the current executable
 * to the maximum possible (higher than any task or ISR) so that nothing that gets
 * activated causes an immediate context switch with locked interrupts
*/
dv_prio_t dv_raiseprio(void)
{
	dv_prio_t old = dv_exe[dv_currentexe].currprio;

	if ( old < dv_maxprio )
	{
		dv_exe[dv_currentexe].currprio = dv_maxprio;
		dv_enqueue(dv_exe[dv_currentexe].currprio, dv_currentexe);
	}

	return old;
}

/* dv_lowerprio() - decrease priority of current executable
 *
 * Kernel functions use this to reduce the priority of the current executable after
 * earlier raising the priority.
*/
void dv_lowerprio(dv_prio_t p)
{
	if ( p == dv_maxprio )
		return;

	if ( dv_dequeue(dv_maxprio) != dv_currentexe )
		dv_panic(dv_panic_QueueCorrupt, dv_sid_scheduler, "current executable was not at head of maxprio queue");

	dv_exe[dv_currentexe].currprio = p;
}


/* dv_activateexe2() - activate an executable, part 2
 *
 * Used in dv_chaintask, when the max. activations check is omitted (chaining self)
*/
static dv_statustype_t dv_activateexe2(dv_id_t e, dv_intstatus_t is)
{
#if DV_DEBUG
	dv_printf("dv_activateexe2() - %d\n", e);
#endif

	dv_exe[e].nact++;
	if ( dv_exe[e].state != dv_running )	/* Self-activation doesn't change the state */
		dv_exe[e].state = dv_ready;

	/* Clear pending events for an extended task
	*/
	dv_clearpending(e);

	/* Enqueue executable
	*/
	dv_enqueue(dv_exe[e].baseprio, e);

	/* Now run any executables that are of higher priority than caller.
	*/
	dv_runqueued_onkernelstack(dv_exe[e].baseprio, dv_exe[dv_currentexe].currprio, is);

	dv_restore(is);
	return dv_e_ok;
}

/* dv_createqueues() - allocate the slots for the queueus
*/
static void dv_createqueues(void)
{
	int s = 0;
	for ( int q = 0; q < DV_NQUEUE; q++ )
	{
		dv_queue[q].slots = &dv_slots[s];
		s += dv_queue[q].nslots;
		if ( s > dv_maxslot )
			dv_panic(dv_panic_ConfigNslotsInsufficient, dv_sid_startos, "insufficient queue slotsi configured");
	}
}

/* dv_calculatelevels() - calculate the levels for IRQs
 *
 * Allocate a lock level > 0 for every queue of higher priority than the highest task. Ignore queues
 * that aren't in use.
*/
static void dv_calculatelevels(void)
{
#if DV_DEBUG
	dv_printf("dv_calculatelevels() - dv_ntask %d dv_nexe %d\n", dv_ntask, dv_nexe);
#endif
	dv_prio_t l = 0;
	for ( dv_prio_t p = dv_maxtaskprio + 1; p <= dv_maxprio; p++ )
	{
#if DV_DEBUG
		dv_printf("dv_calculatelevels() - prio %d\n", p);
#endif
		int c = 0;
		for ( dv_id_t e = dv_ntask; e < dv_nexe; e++ )
		{
#if DV_DEBUG
			dv_printf("dv_calculatelevels() - prio %d, exe %d", p, e);
#endif
			if ( dv_exe[e].baseprio == p )
			{
				c++;
#if DV_DEBUG
				dv_printf("\ndv_calculatelevels() - confg_irq exe %d, irqid %d, level l\n", e, dv_exe[e].irqid, l);
#endif
				dv_config_irq(dv_exe[e].irqid, l, 0);
			}
#if DV_DEBUG
			else
				dv_printf(" ... not at this prio\n");
#endif
		}
		if ( c > 0 )
			l++;
		dv_queue[p].level = l;
#if DV_DEBUG
		dv_printf("dv_calculatelevels() - queue prio %d, level %d\n", p, dv_queue[p].level);
#endif
	}
}

/* dv_panic() - report a fatal error
*/
void dv_panic(dv_panic_t p, dv_sid_t sid, char *fault)
{
	callout_panic(p, sid, fault);
	for (;;) ;
}

/* dv_idle() - the idle loop
*/
static void dv_idle(void)
{
	for (;;)
	{
		callout_idle();
	}
}

/* ===================================================================================================================
 * Interrupt vectoring functions
 * ===================================================================================================================
*/

/* dv_unconfigured_interrupt() - report an unconfigured interrupt
*/
dv_statustype_t dv_unconfigured_interrupt(dv_id_t p)
{
	dv_panic(dv_panic_UnconfiguredInterrupt, dv_sid_interruptdispatcher, "an unconfigured interrupt has occurred");
	return dv_e_id;
}

/* dv_initvectors() - initialise all the interrupt vectors to "unconfigured"
*/
static void dv_initvectors(void)
{
	for ( int i = 0; i < DV_NVECTOR; i++ )
	{
		dv_vectors[i].fn = &dv_unconfigured_interrupt;
		dv_vectors[i].p = i;
	}
}
