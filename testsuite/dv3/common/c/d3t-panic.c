/* d3t-panic.c - panic function for davros-3 test suite
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
 *
 *
*/
#include <kernel/h/dv-kconfig.h>
#include <dv-projectconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-error.h>

void dv_panic(dv_panic_t reason, char *function, char *message)
{
    dv_kprintf("!!! Panic %d in %s: %s\n", reason, function, message);
    for (;;) {}
}
