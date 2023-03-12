/* tusb_os_davroska.h - davroska osal for tinyusb
 *
 * This file contains a tinyusb OSAL for davroska
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef tusb_os_davroska_h
#define tusb_os_davroska_h

#include "dv-config.h"
#include <davroska.h>

/* davroska is configured at startup.
 *
 * You will need to create the following objects at startup:
 *
 *	- a counter. This can be the project's main counter; provide its ID using TUSB_COUNTERID
 *	- a mutex called tusb_Mutex
 *	- for a USB host:
 *		- an extended task called tusb_HostTask that uses tusb_Mutex
 *		- an alarm called tusb_HostAlarm that calls tusb_Expiry on expiry, with data tusb_HostTask
 *	- for a USB device:
 *		- an extended task called tusb_DeviceTask that uses tusb_Mutex
 *		- an alarm called tusb_DeviceAlarm that calls tusb_Expiry on expiry, with data tusb_DeviceTask
 *	- as many ISRs (category 2) as needed by the hardware.
 *		- each ISR uses the tusb_Mutex resource
 *		- each ISR calls tud_int_handler() and/or tuh_int_handler() as appropriate, with the port ID as parameter.
 *
 * In addition to the davroska configuration, you need to define OSAL_OPT_HOST and/or OSAL_OPT_DEVICE externally.
 * This avoids referencing symbols that don't exist in the generated OS.
*/

#ifndef OSAL_OPT_HOST
#define OSAL_OPT_HOST	0
#endif
#ifndef OSAL_OPT_DEVICE
#define OSAL_OPT_DEVICE	0
#endif

#ifndef OSAL_DEBUG
#define OSAL_DEBUG	0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern dv_id_t tusb_HostTask, tusb_HostAlarm, tusb_DeviceTask, tusb_DeviceAlarm, tusb_Mutex;
extern dv_u64_t tusb_Expiry(dv_id_t alarmId, dv_param_t taskId);
extern dv_id_t TUSB_COUNTERID;

#define tusb_EvTimeout	0x01
#define tusb_EvNotify	0x02

//--------------------------------------------------------------------+
// Some generic functions
//--------------------------------------------------------------------+
static inline bool osal_wgcevent(bool is_host, uint32_t msec)
{
	if ( msec == OSAL_TIMEOUT_WAIT_FOREVER )
	{
		(void)dv_waitevent(tusb_EvNotify);
		(void)dv_clearevent(tusb_EvNotify);

		return 1;
	}
	else
	{
		dv_id_t alarm = -1;
		dv_id_t task = -1;
#if OSAL_OPT_HOST
		if ( is_host )
		{
			alarm = tusb_HostAlarm;
			task = tusb_HostTask;
		}
#endif
#if OSAL_OPT_DEVICE
		if ( !is_host )
		{
			alarm = tusb_DeviceAlarm;
			task = tusb_DeviceTask;
		}
#endif

		dv_eventmask_t evt;
		(void)dv_setalarm_rel(TUSB_COUNTERID, alarm, msec);
		(void)dv_waitevent(tusb_EvNotify | tusb_EvTimeout);
		(void)dv_getevent(task, &evt);
		if ( (evt & tusb_EvTimeout) == 0 )
		{
			(void)dv_stopalarm(alarm);
		}
		(void)dv_clearevent(tusb_EvNotify | tusb_EvTimeout);

		return (evt & tusb_EvNotify) != 0;
	}
}

//--------------------------------------------------------------------+
// TASK API
//
// This implementation assumes a counter tick rate of 1 kHz
// osal_task_delay() appears to be used in host only.
//--------------------------------------------------------------------+
static inline void osal_delay(bool is_host, uint32_t msec)
{
#if OSAL_OPT_HOST
	if ( is_host )
	{
		(void)dv_setalarm_rel(TUSB_COUNTERID, tusb_HostAlarm, msec);
		(void)dv_waitevent(tusb_EvTimeout);
		(void)dv_clearevent(tusb_EvTimeout);
	}
#endif
#if OSAL_OPT_DEVICE
	if ( !is_host )
	{
		(void)dv_setalarm_rel(TUSB_COUNTERID, tusb_DeviceAlarm, msec);
		(void)dv_waitevent(tusb_EvTimeout);
		(void)dv_clearevent(tusb_EvTimeout);
	}
#endif
}

static inline void osal_task_delay(uint32_t msec)
{
	osal_delay(1, msec);
}

//--------------------------------------------------------------------+
// Semaphore API
//
// The semaphore API only appears to be used in the CDC class, which
// appears to be work-in-progress. Assume it's host for now.
//--------------------------------------------------------------------+
typedef void osal_semaphore_def_t;
typedef int osal_semaphore_t;

static inline osal_semaphore_t osal_semaphore_create(osal_semaphore_def_t* semdef)
{
	return 0;			// OSEK is statically configured
}

static inline bool osal_semaphore_post(osal_semaphore_t sem_hdl, bool in_isr)
{
#if OSAL_OPT_HOST
	dv_statustype_t res = dv_setevent(tusb_HostTask, tusb_EvNotify);
	return res == dv_e_ok;
#else
	return 0;
#endif
}

static inline bool osal_semaphore_wait(osal_semaphore_t sem_hdl, uint32_t msec)
{
#if OSAL_OPT_HOST
	return osal_wgcevent(1, msec);
#else
	return 0;
#endif
}

static inline void osal_semaphore_reset(osal_semaphore_t const sem_hdl)
{
	// Nothing to do here. Can only call dv_clearevent() from the task.
}

//--------------------------------------------------------------------+
// MUTEX API (priority inheritance)
//--------------------------------------------------------------------+
typedef dv_id_t osal_mutex_def_t;
typedef dv_id_t osal_mutex_t;

static inline osal_mutex_t osal_mutex_create(osal_mutex_def_t* mdef)
{
	return tusb_Mutex;		// Return the ID of the statically-configured resource
}

static inline bool osal_mutex_lock(osal_mutex_t mutex_hdl, uint32_t msec)
{
	dv_statustype_t s = dv_takemutex(mutex_hdl);
	return s == dv_e_ok;
}

static inline bool osal_mutex_unlock(osal_mutex_t mutex_hdl)
{
	dv_statustype_t s = dv_dropmutex(mutex_hdl);
	return s == dv_e_ok;
}

//--------------------------------------------------------------------+
// QUEUE API
//--------------------------------------------------------------------+

// _int_set is not used in davroska
#define OSAL_QUEUE_DEF(_int_set, _name, _depth, _type) \
	static _type _name##_##buf[_depth+1];	\
	osal_queue_def_t _name =				\
	{	.len = (_depth+1),					\
		.blocksize = sizeof(_type),			\
		.buf = (uint8_t*)_name##_##buf		\
	};

typedef struct
{
	uint16_t			len;		// No of data blocks = depth+1 (see below)
	uint16_t			blocksize;	// Size of each data block
	uint8_t*			buf;		// Buffer area

	volatile uint16_t	w_index;	// Writer index.	Queue is empty when w_index == r_index
	volatile uint16_t	r_index;	// Reader index.    Queue is full when (w_index+1) mod len == r_index
} osal_queue_def_t;

typedef osal_queue_def_t *osal_queue_t;

static inline osal_queue_t osal_queue_create(osal_queue_def_t* qdef)
{
	return qdef;
}

#if OSAL_DEBUG
extern bool extern_queue_receive(osal_queue_t qhdl, void* data);
extern bool extern_queue_send(osal_queue_t qhdl, void const * data, bool in_isr);
#endif

static inline bool inline_queue_receive(osal_queue_t qhdl, void* data)
{
	if ( qhdl->r_index == qhdl->w_index )
	{
		(void)dv_waitevent(tusb_EvNotify);
		(void)dv_clearevent(tusb_EvNotify);

		if ( qhdl->r_index == qhdl->w_index )
			return 0;				// Should never happen
	}

	const uint8_t *src = (const uint8_t *)&qhdl->buf[qhdl->r_index * qhdl->blocksize];
	volatile uint8_t *dest = (volatile uint8_t *)data;

	for ( int i = 0; i < qhdl->blocksize; i++ )
		*dest++ = *src++;

	uint16_t newidx = qhdl->r_index + 1;
	if ( newidx >= qhdl->len )
		newidx = 0;
	qhdl->r_index = newidx;

	return 1;
}

static inline bool osal_queue_receive(osal_queue_t qhdl, void* data, uint32_t msec)
{
	(void)msec;		/* For now: always wait forever */
#if OSAL_DEBUG
	return extern_queue_receive(qhdl, data);
#else
	return inline_queue_receive(qhdl, data);
#endif
}

static inline bool inline_queue_send(osal_queue_t qhdl, void const * data, bool in_isr)
{
	uint16_t newidx = qhdl->w_index + 1;
	if ( newidx >= qhdl->len )
		newidx = 0;

	if ( newidx == qhdl->r_index )
		return 0;					// Queue full

	volatile uint8_t *src = (volatile uint8_t *)data;
	volatile uint8_t *dest = (volatile uint8_t *)&qhdl->buf[qhdl->w_index * qhdl->blocksize];

	for ( int i = 0; i < qhdl->blocksize; i++ )
		*dest++ = *src++;

	qhdl->w_index = newidx;

#if OSAL_OPT_DEVICE
	(void)dv_setevent(tusb_DeviceTask, tusb_EvNotify);
#endif

#if OSAL_OPT_HOST
	(void)dv_setevent(tusb_HostTask, tusb_EvNotify);
#endif

	return 1;
}

static inline bool osal_queue_send(osal_queue_t qhdl, void const * data, bool in_isr)
{
#if OSAL_DEBUG
	return extern_queue_send(qhdl, data, in_isr);
#else
	return inline_queue_send(qhdl, data, in_isr);
#endif
}

static inline bool osal_queue_empty(osal_queue_t qhdl)
{
	return (qhdl->w_index == qhdl->r_index);
}

#ifdef __cplusplus
}
#endif

#endif
