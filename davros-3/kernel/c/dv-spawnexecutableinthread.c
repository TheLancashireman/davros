/*	dv-spawnexecutableinthread.c - spawn or resume an executable in directly in its thread.
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
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-doublylinkedlist.h>
#include DV_H_REGISTERS

DV_COVDEF(spawn_executable_in_thread);

/* dv_resume_executable_in_thread() - resume an executable in the specified thread.
 *
 * Program the thread with the executable's data, then enqueue it.
 *
 * On entry:
 *	- the thread is idle
*/
void dv_resume_executable_in_thread(dv_doublylinkedlist_t *dll, dv_executable_t *exe, dv_thread_t *thr)
{
	thr->executable = exe;
	thr->parent = DV_NULL;
	thr->state = dv_thread_new;
	thr->regs = exe->registers;
	dv_set_prio(thr, exe->baseprio);
	dv_dllinsertaftersame(dll, &thr->link);
}

/* dv_spawn_executable_in_thread() - spawn an executable in the specified thread.
 *
 * Program the thread with the executable's data, then enqueue it.
 *
 * On entry:
 *	- the thread is idle
*/
void dv_spawn_executable_in_thread(dv_doublylinkedlist_t *dll, dv_executable_t *exe, dv_thread_t *thr)
{
	dv_set_registers(exe->registers, exe);
	dv_resume_executable_in_thread(dll, exe, thr);
}
