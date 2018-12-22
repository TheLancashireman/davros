/* davroska.c - OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#include <dv-config.h>
#include <davroska-api.h>
#include <davroska.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER

#define DV_NQUEUE	(DV_CFG_MAXPRIO+2)			/* Add extra queues for idle and kernel */
#define DV_NEXE		(DV_CFG_MAXEXE+1)			/* Add an extra executable for the idle loop */
#define DV_NSLOT	(DV_NQUEUE*2 + DV_NEXE + DV_CFG_NSLOT_EXTRA)

const dv_qty_t dv_maxexe = DV_NEXE;
const dv_qty_t dv_maxprio = DV_CFG_MAXPRIO;
const dv_qty_t dv_maxslot = DV_NSLOT+1;
const dv_qty_t dv_maxlock = DV_CFG_MAXLOCK;

dv_id_t dv_currenttask;							/* Convenience variable for GetTaskID and GetISRID */
dv_id_t dv_currentisr;							/* Convenience variable for GetISRID */
dv_prio_t dv_currentprio;						/* Current priority */
dv_prio_t dv_highestprio;						/* Highest priority in use */

static dv_qty_t dv_nexe;						/* No. of executables created */
static dv_qty_t dv_ntask;						/* No. of tasks created */
static dv_qty_t dv_nisr;						/* No. of ISRs created */
static dv_qty_t dv_nlock;						/* No. of locks dreated */

static dv_id_t dv_currentexe;					/* Current executable */
static dv_prio_t dv_maxtaskprio;				/* Highest-priority task */

static dv_exe_t dv_exe[DV_NEXE];				/* Executables */
static dv_q_t dv_queue[DV_NQUEUE];				/* Priority queues (one per priority) */
static dv_id_t dv_slots[DV_NSLOT];				/* Ring buffers for the queues */
static dv_lock_t dv_lock[DV_CFG_MAXLOCK];		/* Locks */

/* Forward function declaractions
*/
static void dv_idle(void);
static void dv_preexehook(void);
static void dv_postexehook(void);
static void dv_panic(dv_panic_t p);
static void dv_createqueues(void);
static dv_statustype_t dv_activateexe(dv_id_t e);
static dv_statustype_t dv_activateexe2(dv_id_t ei, dv_intstatus_t is);
static void dv_runexe(dv_id_t e, dv_intstatus_t is);
static dv_statustype_t dv_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nParam, dv_param_t *p);
static dv_id_t dv_addexe(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);

/* Interrupt vectoring
*/
static int dv_isvectorfree(dv_id_t vec);
static void dv_setvector(dv_id_t vec, dv_statustype_t (*fn)(dv_id_t p), dv_id_t p);
static void dv_initvectors(void);

/* ===================================================================================================================
 * Static inline functions
 * ===================================================================================================================
*/

/*	dv_enqueue() - insert executable at tail of priority queue
*/
static inline void dv_enqueue(dv_prio_t p, dv_id_t e)
{
	dv_q_t *q = &dv_queue[p];

	q->slots[q->tail] = e;

	q->tail++;
	if ( q->tail >= q->nslots )
		q->tail = 0;

	/* Sanity check
	*/
	if ( q->tail == q->head )	
		dv_panic(dv_panic_QueueOverflow);
}

/* dv_qhead() - return head of queue without removing it
*/
static inline dv_id_t dv_qhead(dv_prio_t p)
{
	dv_q_t *q = &dv_queue[p];

	if ( q->head == q->tail )
		return -1;

	return q->slots[q->head];
}

/* dv_dequeue() - remove executable from head of priority queue
*/
static inline dv_id_t dv_dequeue(dv_prio_t p)
{
	dv_q_t *q = &dv_queue[p];

	if ( q->head == q->tail )
		return -1;

	dv_id_t e = q->slots[q->head];
	q->slots[q->head] = -1;

	q->head++;
	if ( q->head >= q->nslots )
		q->head = 0;

	return e;
}

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
	if ( (t < 1) || (t > dv_ntask) )
	{
		dv_param_t p = (dv_param_t)t;
		return dv_reporterror(dv_sid_activatetask, dv_e_id, 1, &p);
	}

	return dv_activateexe(t);
}

/* dv_chaintask() - terminate the called and activate a task
 *
 * If it's a task, terminate me then activate it.
*/
dv_statustype_t dv_chaintask(dv_id_t t)
{
	if ( (t < 1) || (t > dv_ntask) )
	{
		dv_param_t p = (dv_param_t)t;
		return dv_reporterror(dv_sid_activatetask, dv_e_id, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( (t != dv_currentexe) && (dv_exe[t].nact >= dv_exe[t].maxact) )
	{
		dv_restore(is);
		dv_param_t p = (dv_param_t)t;
		return dv_reporterror(dv_sid_activatetask, dv_e_limit, 1, &p);
	}

	/* Sanity checks
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_CurrentExeCorrupt);

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_CurrentExeDead);

	(void)dv_activateexe2(t, is);

	dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

	dv_panic(dv_panic_ReturnFromLongjmp);
}

/* dv_terminatetask() - terminate the calling task
*/
dv_statustype_t dv_terminatetask(void)
{
	(void)dv_disable();

	/* Sanity checks
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_CurrentExeCorrupt);

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_CurrentExeDead);

	dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

	dv_panic(dv_panic_ReturnFromLongjmp);
}

/* dv_takelock() - take a lock
 *
 * The lock's ceiling priority must be higher than the caller's base priority
*/
dv_statustype_t dv_takelock(dv_id_t lock)
{
	if ( (lock < 0) || (lock >= dv_nlock) )
	{
		dv_param_t p = (dv_param_t)lock;
		return dv_reporterror(dv_sid_takelock, dv_e_id, 1, &p);
	}

	if ( dv_lock[lock].ceiling <= dv_exe[dv_currentexe].baseprio )
	{
		dv_param_t p = (dv_param_t)lock;
		return dv_reporterror(dv_sid_takelock, dv_e_access, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_lock[lock].owner == dv_currentexe )
	{
		if ( dv_lock[lock].ntake >= dv_lock[lock].maxtake )
		{
			(void)dv_restore(is);
			dv_param_t p = (dv_param_t)lock;
			return dv_reporterror(dv_sid_takelock, dv_e_nesting, 1, &p);
		}
		else
		{
			dv_lock[lock].ntake++;
			(void)dv_restore(is);
			return dv_e_ok;
		}
	}

	if ( dv_lock[lock].owner >= 0 )			/* Indicates that an exe of lower priority than the owner is running */
		dv_panic(dv_panic_LockOccupied);

	/* Mark lock as owned by current executable
	*/
	dv_lock[lock].owner = dv_currentexe;
	dv_lock[lock].ntake = 1;

	/* Insert the lock at the head of the current executable's list
	*/
	dv_lock[lock].next = dv_exe[dv_currentexe].locklist;
	dv_exe[dv_currentexe].locklist = lock;

	/* Save the current priority in the lock and increase the executable's priority to the ceiling.
	 * Put executable into ceiling queue if the priority actually increases
	*/
	dv_lock[lock].saveprio = dv_currentprio;
	if ( dv_currentprio < dv_lock[lock].ceiling )
	{
		dv_currentprio = dv_exe[dv_currentexe].currprio = dv_lock[lock].ceiling;
		dv_enqueue(dv_currentprio, dv_currentexe);
	}

	(void)dv_restore(is);
	return dv_e_ok;
}

/* dv_droplock() - drop a lock
 *
 * The lock must have been taken by the executable, and if it is the last drop (ntake will become zero)
 * the lock must be the most recently taken lock (i.e. at the head of the exe's lock list)
*/
dv_statustype_t dv_droplock(dv_id_t lock)
{
	if ( (lock < 0) || (lock >= dv_nlock) )
	{
		dv_param_t p = (dv_param_t)lock;
		return dv_reporterror(dv_sid_droplock, dv_e_id, 1, &p);
	}

	dv_intstatus_t is = dv_disable();

	if ( dv_lock[lock].owner != dv_currentexe )
	{
		dv_param_t p = (dv_param_t)lock;
		dv_restore(is);
		return dv_reporterror(dv_sid_droplock, dv_e_nofunc, 1, &p);
	}

	if ( dv_lock[lock].ntake > 1 )
	{
		dv_lock[lock].ntake--;
		dv_restore(is);
		return dv_e_ok;
	}

	dv_lock[lock].ntake = 0;
	dv_lock[lock].owner = -1;
	dv_exe[dv_currentexe].locklist = dv_lock[lock].next;
	dv_lock[lock].next = -1;

	dv_exe[dv_currentexe].currprio = dv_lock[lock].saveprio;

	if ( dv_lock[lock].saveprio < dv_lock[lock].ceiling )
	{
		dv_prio_t p = dv_lock[lock].saveprio;
		dv_id_t me = dv_currentexe;

		if ( dv_dequeue(dv_lock[lock].ceiling) != me )
			dv_panic(dv_panic_QueueCorrupt);

		dv_runqueued(dv_lock[lock].saveprio, is);

		/* When all higher-priority activity is done, back to the original caller
		*/
		dv_currentprio = p;
		dv_currentexe = me;
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

	/* Initialise the interrupt vectors
	*/
	dv_initvectors();

	/* Ensure that no executables can preempt until everything is initialised
	*/
	dv_currentprio = dv_maxprio;

	/* Create the idle loop executable
	*/
	dv_id_t idle = dv_addexe("idle-loop", &dv_idle, 0, 1);

	if ( idle < 0 )
		dv_panic(dv_panic_UnknownPanic);		/* ToDo: proper panic code */

	dv_ntask = 1;	/* Range check for tasks is from 1 to dv_ntask */

	callout_addtasks(mode);

	callout_addisrs(mode);

	dv_createqueues();

	callout_addlocks(mode);

	dv_activateexe(idle);

	callout_autostart(mode);

	dv_exe[idle].state = dv_ready;
	dv_runqueued(0, DV_INTENABLED);

	/* If the task - or the other tasks that it starts - ever die,
	 * we enable interrupts and drop into the idle loop waiting for an interrupt.
	*/
	dv_currentexe = 0;
	dv_currentprio = 0;
	dv_exe[0].state = dv_running;
	dv_restore(DV_INTENABLED);
	dv_idle();

	/* To keep the compiler happy
	*/
	return dv_e_ok;
}

/* ===================================================================================================================
 * Configuration API functions
 * ===================================================================================================================
*/

/* dv_addtask() - add a task to the set of executables
*/
dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
{
	if ( maxact < 1 )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Warning - task with maxaxt 0 or less defaults to 1\n", name);
		maxact = 1;
	}

	if ( prio < 1 )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Error - task with priority 0 or less will never run\n", name);
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
	if ( prio <= dv_maxtaskprio )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addisr(%s,...) :: Error - ISR with priority less than or equal to highest task\n", name);
		return -1;
	}

	if ( !dv_isvectorfree(irqid) )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addisr(%s,...) :: Error - Vector is already in use.\n", name);
		return -1;
	}

	dv_id_t id = dv_addexe(name, fn, prio, 1);

	if ( id >= 0 )
	{
		dv_nisr++;
		dv_setvector(irqid, dv_activateexe, id);
	}

	return id;
}

/* dv_addlock() - add a lock to the set of locks
 *
 * The ceiling priority is detemined later....
*/
dv_id_t dv_addlock(const char *name, dv_qty_t maxtake)
{
	if ( dv_nlock >= dv_maxlock )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addlock(%s,...) :: Config error - DV_CFG_MAXLOCK is insufficient\n", name);
		return -1;
	}

	if ( maxtake < 1 )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addlock(%s,...) :: Warning - lock with maxtake 0 or less defaults to 1\n", name);
		maxtake = 1;
	}

	dv_id_t id = dv_nlock++;

	dv_lock[id].name = name;
	dv_lock[id].ceiling = 0;
	dv_lock[id].maxtake = maxtake;
	dv_lock[id].ntake = 0;
	dv_lock[id].owner = -1;
	dv_lock[id].saveprio = -1;
	dv_lock[id].next = -1;

	return id;
}

/* dv_addlockuser() - add an executable to the notional list of users of a lock
 *
 * The ceiling priority of the lock is determined by one or more calls to this function
*/
void dv_addlockuser(dv_id_t l, dv_id_t e)
{
	if ( (l < 0) || (l >= dv_nlock) )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addlockuser(%d, %d) :: Error - lock doesn't exist\n", l, e);
	}

	if ( (e < 0) || (e >= dv_nexe) )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addlockuser(%d, %d) :: Error - executable doesn't exist\n", l, e);
	}

	if ( dv_lock[l].ceiling < dv_exe[e].baseprio )
		dv_lock[l].ceiling = dv_exe[e].baseprio;
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
		/* ToDo: dv_reporterror */
		dv_printf("dv_addexe(%s,...) :: Config error - DV_CFG_MAXEXE is insufficient\n", name);
		return -1;
	}

	if ( prio > dv_maxprio )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addexe(%s,...) :: Error - priority exceeds DV_CFG_MAXPRIO\n", name);
		return -1;
	}

	dv_id_t id = dv_nexe++;

	dv_exe[id].name = name;
	dv_exe[id].func = fn;
	dv_exe[id].maxact = maxact;
	dv_exe[id].nact = 0;
	dv_exe[id].baseprio = prio;
	dv_exe[id].currprio = prio;
	dv_exe[id].state = dv_suspended;
	dv_exe[id].locklist = -1;

	dv_queue[prio].nslots += maxact;

	return id;
}

/* dv_activateexe() - activate an executable
 *
 * If the task has a higher priority than the caller, call it.
 * Otherwise enqueue it.
*/
static dv_statustype_t dv_activateexe(dv_id_t e)
{
	dv_intstatus_t is = dv_disable();

	/* Max activations check
	*/
	if ( dv_exe[e].nact >= dv_exe[e].maxact )
	{
		dv_restore(is);
		dv_param_t p = (dv_param_t)e;
		return dv_reporterror(dv_sid_activatetask, dv_e_limit, 1, &p);
	}

	/* Go to part 2
	*/
	return dv_activateexe2(e, is);
}

/* dv_runqueued() - run all higher-priority executables
*/
void dv_runqueued(dv_qty_t p, dv_intstatus_t is)
{
	while ( dv_currentprio > p )
	{
		dv_id_t e = dv_qhead(dv_currentprio);
		if ( e == 0 )
		{
			/* The idle executable is never in a higher-priority queue
			*/
			dv_panic(dv_panic_QueueCorrupt);
		}
		else
		if ( e > 0 )
		{
			/* Run the excutable. When it finally terminates it gets removed from the queue
			*/
			dv_runexe(e, is);
		}
		else
		{
			/* Queue is empty. Drop down to next lower priority.
			*/
			dv_currentprio--;
		}
	}
}

/* dv_activateexe2() - activate an executable, part 2
 *
 * Used in dv_chaintask, when the max. activations check is omitted (chaining self)
*/
static dv_statustype_t dv_activateexe2(dv_id_t e, dv_intstatus_t is)
{
	dv_exe[e].nact++;

	if ( dv_exe[e].state != dv_running )	/* Self-activation doesn't change the state */
		dv_exe[e].state = dv_ready;

	/* Enqueue executable
	*/
	dv_enqueue(dv_exe[e].baseprio, e);

	/* Priority check: return if not preempted
	*/
	if ( dv_exe[e].baseprio <= dv_currentprio )
	{
		dv_restore(is);
		return dv_e_ok;
	}

	/* Post-exe hook for outgoing exe
	*/
	dv_postexehook();

	/* Remember current priority and set the new priority
	*/
	dv_qty_t p = dv_currentprio;
	dv_currentprio = dv_exe[e].baseprio;

	/* Remember current executable
	*/
	dv_id_t me = dv_currentexe;

	/* Run the incoming exe
	*/
	dv_runexe(e, is);

	/* After that little excursion there might be lots more executables waiting to run.
	 * Those that are of higher priority than the caller have to run here.
	*/
	dv_runqueued(p, is);

	/* When all higher-priority activity is done, back to the original caller
	*/
	dv_currentprio = p;
	dv_currentexe = me;
	dv_preexehook();
	dv_restore(is);
	return dv_e_ok;
}

/* dv_runexe() - execute an executable
 *
 * Execute a task or ISR that is either:
 *		- preempting the current task or ISR, or
 *		- the next most eligible task or ISR after a termination.
*/
static void dv_runexe(dv_id_t e, dv_intstatus_t is)
{
	dv_jmpbuf_t jb;

	if ( dv_setjmp(jb) == 0 )	/* TerminateTask() causes non-zero return here */
	{
		/* Set outgoing exe to READY
		*/
		dv_exe[dv_currentexe].state = dv_ready;

		/* Switch to incoming exe, set to RUNNING
		*/
		dv_currentexe = e;
		dv_currentprio = dv_exe[dv_currentexe].currprio = dv_exe[dv_currentexe].baseprio;
		dv_exe[e].state = dv_running;
		dv_exe[e].jb = &jb;

		/* Todo: raise priority to for internal resources, non-preemptable, etc.
		*/

		/* Pre-exe hook for incoming exe
		*/
		dv_preexehook();

		/* Interrupt state of caller is preserved for incoming exe
		*/
		dv_restore(is);

		/* Run the exe
		*/
		(*dv_exe[e].func)();
	}

	/* On return or termination don't propagate the interrupt state
	*/
	(void)dv_disable();

	/* Post-exe hook for terminated exe.
	*/
	dv_postexehook();
	dv_exe[e].nact--;
	if ( dv_exe[e].nact <= 0 )
		dv_exe[e].state = dv_suspended;
	
	dv_exe[e].state = dv_suspended;
	dv_exe[e].jb = DV_NULL;

	/* Dequeue the executable (with sanity check)
	*/
	if ( dv_dequeue(dv_currentprio) != dv_currentexe )
		dv_panic(dv_panic_QueueCorrupt);
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
			dv_panic(dv_panic_ConfigNslotsInsufficient);
	}
}

/* dv_preexehook() - call the PreTaskHook or PreISRHook if configured
*/
static void dv_preexehook(void)
{
	/* Todo: implement */
}

/* dv_postexehook() - call the PostTaskHook or PostISRHook if configured
*/
static void dv_postexehook(void)
{
	/* Todo: implement */
}

/* dv_reporterror() - report an error
*/
static dv_errorinfo_t dv_lasterror;

static dv_statustype_t dv_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nparam, dv_param_t *param)
{
	dv_lasterror.sid = sid;
	dv_lasterror.e = e;
	for (int i = 0; i < 4; i++ )
	{
		if ( i < nparam )
			dv_lasterror.p[i] = param[i];
		else
			dv_lasterror.p[i] = 0;
	}
	dv_lasterror.culprit = dv_currentexe;

	callout_error(e);

	return e;
}

/* dv_panic() - report a fatal error
*/
static void dv_panic(dv_panic_t p)
{
	/* Todo: implement */
	dv_printf("dv_panic(%d) called\n", p);
	for (;;) ;
}

/* dv_idle() - the idle loop
*/
static void dv_idle(void)
{
	dv_printf("dv_idle() reached\n");
	for (;;) { }
}

/* ===================================================================================================================
 * Interrupt vectoring functions
 * ===================================================================================================================
*/
static struct
{
	dv_statustype_t (*fn)(dv_id_t p);
	dv_id_t p;
} dv_vectors[DV_NVECTOR];

/* dv_unconfigured_interrupt() - report an unconfigured interrupt
*/
static dv_statustype_t dv_unconfigured_interrupt(dv_id_t p)
{
	dv_panic(dv_panic_UnconfiguredInterrupt);
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

/* dv_isvectorfree() - return true if the vector is in range an is unconfigured, false otherwise
*/
static int dv_isvectorfree(dv_id_t vec)
{
	if ( (vec < 0) || (vec >= DV_NVECTOR) )
		return 0;

	if ( dv_vectors[vec].fn == &dv_unconfigured_interrupt )
		return 1;

	return 0;
}

/* dv_setvector() - set the specified interupt vector
*/
static void dv_setvector(dv_id_t vec, dv_statustype_t (*fn)(dv_id_t p), dv_id_t p)
{
	dv_vectors[vec].fn = fn;
	dv_vectors[vec].p = p;
}

/* dv_softvector() - called from the interrupt controller's interrupt handling function
*/
void dv_softvector(int vector)
{
	(void)(*dv_vectors[vector].fn)(dv_vectors[vector].p);
}
