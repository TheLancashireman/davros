/* davroska.c - OSEK-like member of the davros family
 *
 * (c) David Haworth
*/

#include <h/davroska.h>

const dv_qty_t dv_maxexe = DV_CFG_MAXEXE;		/* Tasks + ISRs */
const dv_qty_t dv_maxprio = DV_CFG_MAXPRIO;		/* Task priorities * interrupt levels */

dv_qty_t dv_nexe;								/* No. of executables created */
dv_qty_t dv_ntask;								/* No. of tasks created */
dv_qty_t dv_nprio;								/* No. of priorities created */

dv_qty_t dv_currenttask;						/* Convenience variables for GetTaskID and GetISRID */
dv_qty_t dv_currentisr;

dv_qty_t dv_currentexe;							/* Current executable */
dv_qty_t dv_currentprio;						/* Current priority */

dv_q_t dv_queue[DV_CFG_MAXPRIO];				/* Priority queues (one per priority) */
dv_exe_t dv_exe[DV_CFG_MAXEXE];				

/* dv_startexe() - execute an executable
 *
 * Execute a task or ISR that is preempting the current task.
*/
static void dv_startexe(dv_id_t e, dv_intstatus_t is)
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

		/* Start the exe
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

/* dv_activatetask() - activate a task
 *
 * If the task has a higher priority than the caller, call it.
 * Otherwise enqueue it.
*/
dv_statustype_t dv_activatetask(dv_id_t e)
{
	if ( (e < 0) || (e >= dv_ntask) )
	{
		return dv_reporterror(dv_sid_activatetask, dv_e_id, e, 0, 0);
	}

	dv_intstatus_t is = dv_disable();

	/* Max activations check
	*/
	if ( dv_exe[e].nact >= dv_exe[e].maxact )
	{
		dv_restore(is);
		return dv_reporterror(dv_sid_activatetask, dv_e_limit, e, 0, 0);
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

	/* Start the incoming exe
	*/
	dv_startexe(e, is);

	/* After that little excursion there might be lots more executables waiting to run.
	 * Those that are of higher priority than the caller have to run here.
	*/
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
			dv_startexe(e, is);
		}
		else
		{
			/* Queue is empty. Drop down to next lower priority.
			*/
			dv_currentprio--;
		}
	}

	dv_preexehook();
	dv_restore(is);
}

/* dv_terminatetask() - terminate the calling task
*/
void dv_terminatetask(void)
{
	/* Sanity check
	*/
	if ( (dv_currentexe < 0) || (dv_currentexe >= dv_nexe) )
		dv_panic(dv_panic_corrupt_currentexe);

	/* ToDo: shall we have a calling context check, resource occupation check etc. here?
	 * Gut feeling says no.
	*/

	/* Sanity check
	*/
	if ( dv_exe[dv_currentexe].jb == DV_NULL )
		dv_panic(dv_panic_currentexe_dead);

	dv_longjmp(*dv_exe[dv_currentexe].jb);

	dv_panic(dv_panic_return_from_longjmp);
}
