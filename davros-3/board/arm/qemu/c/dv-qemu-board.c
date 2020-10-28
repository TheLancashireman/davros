/*	dv-qemu-board.c - QEMU board start for davros
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
#include <kernel/h/dv-kernel.h>
#include <board/arm/qemu/h/dv-qemu-uart.h>
#include <kernel/h/dv-stdio.h>

void dv_board_start(int core_index)
{
	dv_uart0_init();

	dv_kprintf("Davros starting on QEMU, core %d\n", core_index);

	dv_start(core_index);
}
