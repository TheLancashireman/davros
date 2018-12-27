/* davroska-inline.h - header file for OSEK-like member of the davros family; static inline functions
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_inline_h
#define dv_davroska__inlineh	1

#include <davroska.h>

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

/* Temporary ...
*/
static inline void dv_setqueueirqlevel(dv_prio_t p)
{
	dv_setirqlevel(dv_queue[p].level);
}

#endif
