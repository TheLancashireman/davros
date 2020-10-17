# Davros

## Dave's real(time) operating system collection

Just for fun.

License - all GPL v3 unless otherwise stated.  See the LICENSE file

### Davroska

A tiny OS with similar features to OSEK. Apart from extended tasks, it runs on a single stack.

Everything runs with full privileges; there is no memory protection.

Works on:
* [Raspberry Pi zero](#Raspberry-Pi-zero)
* [Raspberry Pi 3](#Raspberry-Pi-3) (64-bit)
* [Blue Pill](#Blue-Pill)

For details, look in the [davroska](davroska/) directory.

### Davros-3

An embedded OS with memory protection. More full-featured than davroska.

Forever under development ;-)

For details, look in the [davros-3](davros-3/) directory.

Works on:
* Raspberry Pi zero. Should also work on the old model A and B boards.
* Should work on the pi3 with a little effort.

## Directory contents

* davroska/ - see above
* davros-3/ - see above
* devices/ - header files and drivers for devices. Used by all davros flavours
* lib/ - some library headers and code. Used by all davros flavours
* demos/ - some demo code to show you how it works
* scripts/ - scripts for starting openocd, minicom, gdb etc.
* LICENSE - a copy of GPL version 3
* README.md - this file

## Supported hardware

### Raspberry Pi zero

* Broadcom BCM2835 SoC based on an arm1176jzf-s core
* Broadcom interrupt controller and peripheral block bolted on the side

### Raspberry Pi 2

Not supported.

* Broadcom BCM2836 SoC based on a quad-core Cortex A7 cluster
* Broadcom QA-7 block with interrupt controller, timers, mailboxes etc. bolted on the side
* Broadcom interrupt controller and peripheral block (same as the pi zero) bolted on the side of the QA-7

### Raspberry Pi 3

* Broadcom BCM2836 SoC based on a quad-core Cortex A53 cluster
* Broadcom QA-7 block with interrupt controller, timers, mailboxes etc. bolted on the side
* Broadcom interrupt controller and peripheral block (same as the pi zero) bolted on the side of the QA-7

### Blue Pill

* STM32F103T8 microcontroller based on a Cortex M3 core

## Links

### Compilers

* <https://www.linaro.org/downloads/>
* <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>

For the Raspberry Pi, the Linaro builds of gcc are probably the best. Grab the bare metal build - aarch64
for the Pi3 (in 64-bit mode), otherwise arm-eabi. You could probably use the linux build with a bit of tweaking.

For the Blue Pill (Cortex-M) the compiler from the arm-eabi build of linaro appears to work and the code
runs until the first setjmp(). It seems that the libraries don't support the Cortex-M's thumb mode properly. So
use the compiler from developer.arm.com. Unfortunately it seems to produce slightly bigger binaries.

### Previous lives of davros

* <https://github.com/BackupTheBerlios/davros-kernel-svn>

Yes, it really has been around that long.

## Why davros?

I used to work for a company that manufactured work-time recording machines and access control systems.
The work-time machines went under the brand name of TimeLord. There was a machine for printing
the cards that the TimeLord used that looked remarkably like Doctor Who's robot dog. So much so,
in fact, that it got the nickname K9.

As part of my work there, I created a XINU-like operating system to run on Hitachi 6303 and similar processors.
It was during that time that I thought of the name Davros, continuing the Doctor Who theme, but the OS
developed for the company I worked for was only ever used internally, so it never needed a name.

Many years later I started fiddling around with my own OS - and of course the name davros seemed like an
obvious choice.

## License, disclaimer etc.

Copyright David Haworth

This file is part of davros.

davros is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

davros is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with davros.  If not, see <http://www.gnu.org/licenses/>.
