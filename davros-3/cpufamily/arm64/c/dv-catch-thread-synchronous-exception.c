/*	dv-catch-thread-synchronous-exception.c - catch a synchronous exception from a thread (arm64)
 *
 *	Copyright 2018 David Haworth
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
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-syscall.h>
#include <cpufamily/arm64/h/dv-arm64-registers.h>
#include <cpufamily/arm64/h/dv-arm64-exceptions.h>

/* dv_catch_thread_synchronous_exception - catch and handle a synchonous exception
 *
 * Preconditions:
 *	- exception entry code has saved all registers to current thread's register store.
 *	- exception syndrome register has been saved in register store too.
 *	- exception entry code has loaded address of kernel variables to x0 (1st parameter).
 *
 * dv_catch_thread_synchronous_exception() must not return.
*/
void dv_catch_thread_synchronous_exception(dv_kernel_t *kvars)
{
	dv_registers_t *regs = kvars->current_thread->regs;
	dv_u32_t esr = regs->esr;
	dv_arm64_ec_t ec = (dv_arm64_ec_t)(esr >> 26);

	if ( ec == dv_ec_svc )
	{
		dv_syscall(kvars, esr & DV_ESR_ISS_SVC);
	}
	else
	{
		dv_panic(dv_panic_unimplemented, "dv_catch_thread_synchronous_exception", "Oops! An exception occurred");
	}
}
