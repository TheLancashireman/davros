/*	dv-resumeexecutable.c - spawn an instance of an executable object
 *
 *	Copyright David Haworth
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
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-ringbuffer.h>
#include <kernel/h/dv-event.h>
#include <kernel/h/dv-error.h>

DV_COVDEF(resume_executable);

/* dv_resume_executable() - resume an instance of an executable
*/
dv_errorid_t dv_resume_executable(dv_kernel_t *kvars, dv_executable_t *exe)
{
	dv_errorid_t ecode = dv_eid_UnknownError;

	exe->state = dv_exe_active;

	if ( exe->thread->state == dv_thread_idle )
	{
		dv_resume_executable_in_thread(&kvars->thread_queue, exe, exe->thread);
		ecode = dv_eid_None;
	}
	else
		ecode = dv_enqueue_job_in_jobqueue(kvars, exe);

	return ecode;
}
