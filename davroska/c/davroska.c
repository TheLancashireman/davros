/* davroska.c - OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#include <dv-config.h>
#include <davroska-api.h>
#include <davroska.h>

#define DV_NQUEUE	(DV_CFG_MAXPRIO+2)			/* Add extra queues for idle and kernel */
#define DV_NEXE		(DV_CFG_MAXEXE+1)			/* Add an extra executable for the idle loop */
#define DV_NSLOT	(DV_NQUEUE*2 + DV_NEXE + DV_CFG_NSLOT_EXTRA)

const dv_qty_t dv_maxexe = DV_NEXE;
const dv_qty_t dv_maxprio = DV_CFG_MAXPRIO;
const dv_qty_t dv_maxslot = DV_NSLOT+1;

static dv_qty_t dv_nexe;						/* No. of executables created */
static dv_qty_t dv_ntask;						/* No. of tasks created */
static dv_qty_t dv_nprio;						/* No. of priorities created */

dv_id_t dv_currenttask;							/* Convenience variables for GetTaskID and GetISRID */
dv_id_t dv_currentisr;

static dv_id_t dv_currentexe;					/* Current executable */
static dv_prio_t dv_currentprio;				/* Current priority */

static dv_exe_t dv_exe[DV_NEXE];				/* Executables */
static dv_q_t dv_queue[DV_NQUEUE];				/* Priority queues (one per priority) */
static dv_id_t dv_slots[DV_NSLOT];				/* Ring buffers for the queues */

/* Forward function declaractions
*/
static void dv_idle(void);
static void dv_preexehook(void);
static void dv_postexehook(void);
static void dv_panic(dv_panic_t p);
static void dv_createqueues(void);
static dv_statustype_t dv_activateexe(dv_id_t e);
static void dv_runexe(dv_id_t e, dv_intstatus_t is);
static void dv_runqueued(dv_qty_t p, dv_intstatus_t is);
static dv_statustype_t dv_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nParam, dv_param_t *p);


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

/* dv_terminatetask() - terminate the calling task
*/
dv_statustype_t dv_terminatetask(void)
{
	/* Sanity check
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_CurrentExeCorrupt);

	/* ToDo: shall we have a calling context check, resource occupation check etc. here?
	 * Gut feeling says no.
	*/

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_CurrentExeDead);

	dv_longjmp(*dv_exe[dv_currentexe].jb, dv_e_longjmp_ok);

	dv_panic(dv_panic_ReturnFromLongjmp);
}

/* dv_startos() - start the OS
*/
dv_statustype_t dv_startos(dv_id_t mode)
{
	/* Initialise the priority queues
	*/
	for (int i = 0; i <= dv_maxprio; i++ )
	{
		dv_queue[i].prio = i;		/* Set the priority */
		dv_queue[i].nslots = 2;		/* Reserve an entry for a raised priority and an entry for the gap */
	}

	/* Ensure that no executables can preempt until everything is initialised
	*/
	dv_currentprio = dv_maxprio;

	/* Create the idle loop executable
	*/
	dv_nexe = 1;
	dv_exe[0].name = "idle-loop";
	dv_exe[0].func = dv_idle;
	dv_exe[0].maxact = 1;
	dv_exe[0].nact = 0;
	dv_exe[0].prio = 0;
	dv_exe[0].state = dv_running;

	callout_addtasks(mode);
#if 0
	callout_addisrs(mode);
#endif

	dv_createqueues();

	dv_activateexe(0);

	callout_autostart(mode);

	dv_exe[0].state = dv_ready;
	dv_runqueued(0, DV_INTENABLED);

	/* If the task - or the other tasks that it starts - ever die,
	 * we enable interrupts and drop into the idle loop waiting for an interrupt.
	*/
	dv_exe[0].state = dv_running;
	dv_restore(DV_INTENABLED);
	dv_idle();
}

/* dv_addtask() - add a task to the set of executables
*/
dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
{
	if ( dv_nexe > dv_maxexe )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Config error - DV_CFG_MAXEXE is insufficient\n", name);
		return -1;
	}

	if ( prio > dv_maxprio )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Error - priority exceeds DV_CFG_MAXPRIO\n", name);
		return -1;
	}

	if ( prio < 1 )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Warning - task with priority 0 or less will never run\n", name);
	}

	if ( maxact < 1 )
	{
		/* ToDo: dv_reporterror */
		dv_printf("dv_addtask(%s,...) :: Warning - task with maxaxt 0 or less defaults to 1\n", name);
		maxact = 1;
	}

	dv_id_t id = dv_nexe++;
	dv_ntask++;

	dv_exe[id].name = name;
	dv_exe[id].func = fn;
	dv_exe[id].maxact = maxact;
	dv_exe[id].nact = 0;
	dv_exe[id].prio = prio;
	dv_exe[id].state = dv_suspended;

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

	dv_exe[e].nact++;

	/* Priority check: call or enqueue
	*/
	dv_q_t *q = &dv_queue[dv_exe[e].prio];

	if ( dv_exe[e].prio <= dv_currentprio )
	{
		/* Task is lower or same priority: enqueue it.
		 *
		 * On 2nd or subsequent concurrent activation of same task this branch is always taken.
		*/
		dv_exe[e].state = dv_ready;

		q->slots[q->tail] = e;

		q->tail++;
		if ( q->tail >= q->nslots )
			q->tail = 0;

		/* Sanity check
		*/
		if ( q->tail == q->head )	
			dv_panic(dv_panic_QueueOverflow);

		dv_restore(is);
		return dv_e_ok;
	}

	/* Post-exe hook for outgoing exe
	*/
	dv_postexehook();

	/* Remember current priority and set the new priority
	*/
	dv_qty_t p = dv_currentprio;
	dv_currentprio = dv_exe[e].prio;

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
	dv_exe[e].state = dv_suspended;
	dv_exe[e].nact--;
	dv_exe[e].jb = DV_NULL;
}

/* dv_runqueued() - run all higher-priority executables
*/
static void dv_runqueued(dv_qty_t p, dv_intstatus_t is)
{
	while ( dv_currentprio > p )
	{
		dv_q_t *q = &dv_queue[dv_currentprio];
		if ( q->head != q->tail )
		{
			/* Remove an executable from head of queue.
			*/
			dv_id_t e = q->slots[q->head];
			q->head++;
			if ( q->head >= q->nslots )
				q->head = 0;

			/* Run the excutable
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
static dv_statustype_t dv_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nparam, dv_param_t *param)
{
	/* Todo: implement */
	dv_printf("dv_reporterror(%d, %d, ...) called\n", sid, e);
	for (int i = 0; i < nparam; i++ )
		dv_printf("    0x%08x\n", param[i]);
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
