/* tusb_os_osek.h - OSEK osal for tinyusb
 *
 * This file contains a tinyusb OSAL for OSEK
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
#ifndef tusb_os_osek_h
#define tusb_os_osek_h

/* OSEK is a statically-configured OS. The configuration, and sometimes the OS code, is usually generated
 * by a configuration program on your PC.
 *
 *
 * In your configuration you need:
 *	- an event called ev_Tusb_Notify
 *	- an event called ev_Tusb_Timeout
 *	- a resource called tusb_Mutex
 *	- for a USB host:
 *		- an extended task called tusb_HostTask that uses the above events and resource
 *		- an alarm called tusb_HostAlarm that sends ev_Tusb_Timeout to tusb_HostTask
 *	- for a USB device:
 *		- an extended task called tusb_DeviceTask that uses the above events and resource
 *		- an alarm called tusb_DeviceAlarm that sends ev_Tusb_Timeout to tusb_DeviceTask
 *	- as many ISRs (category 2) as needed by the hardware.
 *		- each ISR uses the tusb_Mutex resource
 *		- each ISR calls tud_int_handler() and/or tuh_int_handler() as appropriate, with the port ID as parameter.
 *
 * It might be possible to use SuspendOSInterrupts() and ResumeOSInterrupts() instead of GetResource()
 * and ReleaseResource(), but that won't work if TinyUSB calls the OSAL inside a critical section.
 *
 * In addition to the OSEK configuration, you need to define OSAL_OPT_HOST and/or OSAL_OPT_DEVICE externally.
 * This avoids referencing symbols that don't exist in the generated OS.
 *
 * Note: the call to CancelAlarm() might cause an error report via your ErrorHook() function. There is no
 * way to avoid this. Implement your ErrorHook() function so that it ignores E_OS_NOFUNC when the service
 * ID is OSServiceId_CancelAlarm.
*/

#include <os.h>

#ifndef OSAL_OPT_HOST
#define OSAL_OPT_HOST	0
#endif
#ifndef OSAL_OPT_DEVICE
#define OSAL_OPT_DEVICE	0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------+
// Some generic functions
//--------------------------------------------------------------------+
static inline bool osal_wgcevent(bool is_host, uint32_t msec)
{
	if ( msec == OSAL_TIMEOUT_WAIT_FOREVER )
	{
		(void)WaitEvent(ev_Tusb_Notify);
		(void)ClearEvent(ev_Tusb_Notify);

		return 1;
	}
	else
	{
		AlarmType alarm = (AlarmType)(-1);
		TaskType task = INVALID_TASK;
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

		EventMaskType evt;
		(void)SetRelAlarm(alarm, msec, 0);
		(void)WaitEvent(ev_Tusb_Notify | ev_Tusb_Timeout);
		(void)GetEvent(task, &evt);
		if ( (evt & ev_Tusb_Timeout) == 0 )
		{
			(void)CancelAlarm(alarm);
		}
		(void)ClearEvent(ev_Tusb_Notify | ev_Tusb_Timeout);

		return (evt & ev_Tusb_Notify) != 0;
	}
}

//--------------------------------------------------------------------+
// TASK API
//
// This implementation assumes a counter tick rate of 1 kHz
// osal_task_delay() appears to be used in host only.
//--------------------------------------------------------------------+
static inline void osal_task_delay(uint32_t msec)
{
#if OSAL_OPT_HOST
	(void)SetRelAlarm(tusb_HostAlarm, msec, 0);
	(void)WaitEvent(ev_Tusb_Timeout);
	(void)ClearEvent(ev_Tusb_Timeout);
#endif
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
	StatusType res = SetEvent(tusb_HostTask, ev_Tusb_Notify);
	return res == E_OK;
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
	// Nothing to do here. Can only call ClearEvent() from the task.
}

//--------------------------------------------------------------------+
// MUTEX API (priority inheritance)
//--------------------------------------------------------------------+
typedef ResourceType osal_mutex_def_t;
typedef ResourceType osal_mutex_t;

static inline osal_mutex_t osal_mutex_create(osal_mutex_def_t* mdef)
{
	return tusb_Mutex;		// Return the ID of the statically-configured resource
}

static inline bool osal_mutex_lock(osal_mutex_t mutex_hdl, uint32_t msec)
{
	StatusType s = GetResource(mutex_hdl);
	return s == E_OK;
}

static inline bool osal_mutex_unlock(osal_mutex_t mutex_hdl)
{
	StatusType s = ReleaseResource(mutex_hdl);
	return s == E_OK;
}

//--------------------------------------------------------------------+
// QUEUE API
//--------------------------------------------------------------------+

// role device/host is used by OS NONE for mutex (disable usb isr) only
// role device/host is used by OSEK for identifying task to send events to.
#define OSAL_QUEUE_DEF(_role, _name, _depth, _type) \
	static _type _name##_##buf[_depth+1];	\
	osal_queue_def_t _name =				\
	{	.len = (_depth+1),					\
		.blocksize = sizeof(_type),			\
		.buf = (uint8_t*)_name##_##buf,		\
		.role = _role						\
	};

typedef struct
{
	uint16_t			len;		// No of data blocks = depth+1 (see below)
	uint16_t			blocksize;	// Size of each data block
	uint8_t*			buf;		// Buffer area
	uint8_t				role;		// OPT_MODE_NONE, OPT_MODE_DEVICE or OPT_MODE_HOST

	volatile uint16_t	w_index;	// Writer index.	Queue is empty when w_index == r_index
	volatile uint16_t	r_index;	// Reader index.    Queue is full when (w_index+1) mod len == r_index
} osal_queue_def_t;

typedef osal_queue_def_t *osal_queue_t;

static inline osal_queue_t osal_queue_create(osal_queue_def_t* qdef)
{
	return qdef;
}

static inline bool osal_queue_receive(osal_queue_t qhdl, void* data)
{
	if ( qhdl->r_index == qhdl->w_index )
	{
		(void)WaitEvent(ev_Tusb_Notify);
		(void)ClearEvent(ev_Tusb_Notify);
	}

	if ( qhdl->r_index == qhdl->w_index )
		return 0;

	volatile uint8_t *src = (volatile uint8_t *)&qhdl->buf[qhdl->r_index * qhdl->blocksize];
	volatile uint8_t *dest = (volatile uint8_t *)data;

	for ( int i = 0; i < qhdl->blocksize; i++ )
		*dest++ = *src++;

	uint16_t newidx = qhdl->r_index + 1;
	if ( newidx >= qhdl->len )
		newidx = 0;
	qhdl->r_index = newidx;

	return 1;
}

static inline bool osal_queue_send(osal_queue_t qhdl, void const * data, bool in_isr)
{
	uint16_t newidx = qhdl->w_index + 1;
	if ( newidx >= qhdl->len )
		newidx = 0;

	if ( newidx == qhdl->r_index )
		return 0;					// Queue full

	volatile uint8_t *src = (volatile uint8_t *)data;
	volatile uint8_t *dest = (volatile uint8_t *)&qhdl->buf[newidx * qhdl->blocksize];

	for ( int i = 0; i < qhdl->blocksize; i++ )
		*dest++ = *src++;

	qhdl->w_index = newidx;

	switch ( qhdl->role )
	{
#if OSAL_OPT_DEVICE
	case OPT_MODE_DEVICE:
		(void)SetEvent(tusb_DeviceTask, ev_Tusb_Notify);
		break;
#endif

#if OSAL_OPT_HOST
	case OPT_MODE_HOST:
		(void)SetEvent(tusb_HostTask, ev_Tusb_Notify);
		break;
#endif

	default:
		break;
	}

	return 1;
}

static inline bool osal_queue_empty(osal_queue_t qhdl)
{
	return (qhdl->w_index == qhdl->r_index);
}

#ifdef __cplusplus
}
#endif

#endif
