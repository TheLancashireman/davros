/* os.h - public header file for davroska
 *
 * (c) David Haworth
*/
#ifndef DV_davrosek_h
#define DV_davrosek_h	1

typedef short dv_qty_t;
typedef short dv_taskid_t;
typedef unsigned short dv_prio_t;

typedef struct dv_q_s
{
	dv_taskid_t *slots;		/* Start of a buffer for the ring buffer for this priority slot. */
    dv_qty_t nslots;		/* No. of slots in this ring buffer */
	dv_qty_t head;			/* Current position */
	dv_qty_t tail;			/* Current insertion position */
    dv_prio_t prio;			/* Priority of this queue */
} dv_q_t;

typedef void (*dv_taskf_t)(void);

typedef struct dv_task_t
{
	dv_taskf_t func;
	dv_qty_t maxact;
	dv_qty_t nact;
	dv_prio_t prio;
};
