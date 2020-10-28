/*	dv-syscall.c - system-call handler for davros
 *
 *	Copyright 2017 David Haworth
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
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-trace.h>

DV_COVDEF(dv_syscall);

const dv_syscall_t dv_syscalltable[DV_N_SYSCALL+1] =
{
	[dv_sc_null]			= { dv_sys_null,			"dv_sys_null"				},
	[dv_sc_exit]			= { dv_sys_exit,			"dv_sys_exit"				},
	[dv_sc_yield]			= { dv_sys_yield,			"dv_sys_yield"				},
	[dv_sc_create_exe]		= { dv_sys_create_exe,		"dv_sys_create_exe"			},
	[dv_sc_destroy_exe]		= { dv_sys_destroy_exe,		"dv_sys_destroy_exe"		},
	[dv_sc_spawn]			= { dv_sys_spawn,			"dv_sys_spawn"				},
	[dv_sc_chain]			= { dv_sys_chain,			"dv_sys_chain"				},
	[dv_sc_terminate]		= { dv_sys_terminate,		"dv_sys_terminate"			},
	[dv_sc_sleep]			= { dv_sys_sleep,			"dv_sys_sleep"				},
	[dv_sc_sleep_until]		= { dv_sys_sleep_until,		"dv_sys_sleep_until"		},
	[dv_sc_suspend]			= { dv_sys_suspend,			"dv_sys_suspend"			},
	[dv_sc_resume]			= { dv_sys_resume,			"dv_sys_resume"				},
	[dv_sc_unknown]			= { dv_sys_unknown,			"dv_sys_unknown"			},
};

#if 0
	[dv_sc_acquire]			= { dv_sys_acquire,			"dv_sys_acquire"			},
	[dv_sc_drop]			= { dv_sys_drop,			"dv_sys_drop"				},
	[dv_sc_event_wait]		= { dv_sys_event_wait,		"dv_sys_event_wait"			},
	[dv_sc_event_wgc]		= { dv_sys_event_wgc,		"dv_sys_event_wgc"			},
	[dv_sc_event_wgc_all]	= { dv_sys_event_wgc_all,	"dv_sys_event_wgc_all"		},
	[dv_sc_event_set]		= { dv_sys_event_set,		"dv_sys_event_set"			},
	[dv_sc_event_get]		= { dv_sys_event_set,		"dv_sys_event_set_async"	},
	[dv_sc_event_clear]		= { dv_sys_event_get,		"dv_sys_event_get"			},
#endif

/* dv_function() - short description
 *
 * Long description
*/
void dv_syscall(dv_kernel_t *kvars, dv_machineword_t sc_index)
{
	dv_fcov(0);

	if ( dv_ccov(1, 2, dv_dllisempty(&kvars->thread_queue)) )
	{
		dv_panic(dv_panic_threadqueueempty, "dv_syscall", "Thread queue is empty");
	}
	else
	if ( dv_ccov(3, 4, (kvars->current_thread == dv_threadqueuehead(kvars))) )
	{
		if ( dv_ccov(5, 6, (sc_index < (dv_machineword_t)DV_N_SYSCALL)) )
		{
			dv_index_t sci = (dv_index_t)sc_index;
			dv_trace_api(kvars->current_thread, sci, &dv_syscalltable[sci]);
			(*dv_syscalltable[sci].function)(kvars, sci);
			dv_trace_api_done(kvars->current_thread, sci, &dv_syscalltable[sci]);
		}
		else
		{
			dv_sys_unknown(kvars, (dv_index_t)sc_index);
		}

		dv_dispatch(kvars);
	}
	else
	{
		dv_panic(dv_panic_currentthreadnotqueuehead, "dv_syscall", "Current thread is not at head of queue");
	}
}
