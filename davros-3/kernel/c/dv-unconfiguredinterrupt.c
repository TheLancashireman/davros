/*	dv-unconfiguredinterrupt.c - unconfigured interrupt handler for davros
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
#include <kernel/h/dv-interrupt.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-stdio.h>

/* dv_unconfigured_interrupt() - handler for unconfigured interrupts
 *
 * This is a function to handle unconfigured interrupts.
 * Unconfigured interrupts are those that have hardware to trigger them but should
 * not occur because they are not in use.
*/
void dv_unconfigured_interrupt(dv_kernel_t *unused_kvars, dv_address_t param)
{
#if 1
	dv_kprintf("dv_unconfigured_interrupt() - 0x%08x\n", (unsigned)param);
#else
	char buf[12];
	int i;

	/* Todo: use a sprintf function to make a better error message.
	*/
	buf[0] = '0';
	buf[1] = 'x';
	buf[10] = '\0';

	for ( i = 0; i < 8; i++ )
	{
		buf[2+i] = (vec >> (i*4));
		if ( buf[2+i] > 9 )
			buf[2+i] += 'a' - 10;
		else
			buf[2+i] += '0';
	}

	dv_panic(dv_panic_hardwareerror, "dv_unconfigured_interrupt", buf);
#endif
}
