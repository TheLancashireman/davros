/* davroska-inline.h - header file for OSEK-like member of the davros family; static inline functions
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_inline_h
#define dv_davroska__inlineh	1

#include <davroska.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif

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
		dv_panic(dv_panic_QueueOverflow, dv_sid_scheduler, "priority queue is full");
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

/* dv_isvectorfree() - return true if the vector is in range an is unconfigured, false otherwise
*/
static inline int dv_isvectorfree(dv_id_t vec)
{
	if ( (vec < 0) || (vec >= DV_NVECTOR) )
		return 0;

	if ( dv_vectors[vec].fn == &dv_unconfigured_interrupt )
		return 1;

	return 0;
}

/* dv_setvector() - set the specified interupt vector
*/
static inline void dv_setvector(dv_id_t vec, dv_statustype_t (*fn)(dv_id_t p), dv_id_t p)
{
#if DV_DEBUG
	dv_printf("dv_setvector() - %d = %d\n", vec, p);
#endif
	dv_vectors[vec].fn = fn;
	dv_vectors[vec].p = p;
}

/* dv_softvector() - called from the interrupt controller's interrupt handling function
*/
static inline void dv_softvector(int vector)
{
#if DV_DEBUG
	dv_printf("dv_softvector() - %d --> %d\n", vector, dv_vectors[vector].p);
#endif

	(void)(*dv_vectors[vector].fn)(dv_vectors[vector].p);

#if DV_DEBUG
	dv_printf("dv_softvector() - return\n");
#endif
}

/* dv_setqueueirqlevel() - set the irq level that matches the current priority
*/
static inline void dv_setqueueirqlevel(dv_prio_t p)
{
	dv_setirqlevel(dv_queue[p].level);
}

/* dv_stackround() - round a number up to the next multiple of DV_CANARY
 *
 * Note: DV_CANARY must be a power of 2.
*/
static inline dv_u32_t dv_stackround(dv_u32_t n)
{
	return (n + DV_CANARY - 1)&~(DV_CANARY-1);
}

/* dv_extendedtask() - returns true if task is extended
*/
static inline dv_boolean_t dv_extendedtask(dv_id_t t)
{
#if DV_CFG_MAXEXTENDED <= 0
	return 0;
#else
	return (dv_exe[t].extended >= 0);
#endif
}

/* dv_taskwaiting() - returns true if extended task is going to or leaving the waiting state
 *
 * t must be an extended task.
 *
 * Note: the executable's state variable is useless for this because the scheduler must
 * ensure that the state is running before dv_preexe() and until after dv_postexe(). So
 * WaitEvent() cannot pre-set the state.
*/
static inline dv_boolean_t dv_taskwaiting(dv_id_t t)
{
#if DV_CFG_MAXEXTENDED <= 0
	return 0;
#else
	return (dv_extended[dv_exe[t].extended].jb != DV_NULL);
#endif
}

/* dv_clearpending() - clears pending events if task is extended
*/
static inline void dv_clearpending(dv_id_t t)
{
#if DV_CFG_MAXEXTENDED > 0
	if ( dv_extendedtask(t) )
		dv_extended[dv_exe[t].extended].events_pending = 0;
#endif
}

/* dv_set_onkernelstack() - ef extended tasks supported, set the on-kernel-stack flag after task termination
*/
static inline void dv_set_onkernelstack(void)
{
#if DV_CFG_MAXEXTENDED > 0
	extern dv_boolean_t dv_onkernelstack;
	dv_onkernelstack = 1;
#endif
}

/* The following functions are stubs if there are no extended tasks
*/
#if DV_CFG_MAXEXTENDED <= 0
static inline  void dv_extended_init(dv_u32_t *stackbase)
{
}

static inline void dv_runextended(dv_id_t unused_e, dv_intstatus_t unused_is)
{
}

static inline void dv_runqueued_onkernelstack(dv_prio_t high, dv_prio_t low, dv_intstatus_t is)
{
	dv_runqueued(high, low, is);
}

#endif

#endif
