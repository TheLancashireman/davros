# Davros

## Dave's real(time) operating system collection

Just for fun.

License - all GPL v3 unless otherwise stated.  See the LICENSE file

### Davroska

A tiny OS with similar features to OSEK. Apart from extended tasks, it runs on a single stack.

Everything runs with full privileges; there is no memory protection.

Works on:
* [Raspberry pi zero](#raspberry-pi-zero). Should also work on the old model A and B boards.
* [Raspberry pi 3](#raspberry-pi-3) (64-bit)
* [Blue Pill](#blue-pIll)

For details, look in the [davroska](davroska/) directory.

### Davros-3

An embedded OS with memory protection. More full-featured than davroska.

Forever under development ;-)

For details, look in the [davros-3](davros-3/) directory.

Works on:
* [Raspberry pi zero](#raspberry-pi-zero). Should also work on the old model A and B boards.
* [Raspberry pi 3](#raspberry-pi-3) (64-bit) - in development

## Directory contents

* davros-3/ - see above
* davroska/ - see above
* demos/ - some demo code to show you how it works
* devices/ - header files and drivers for devices. Used by all davros flavours
* doc/ - some development notes and other documentation
* lib/ - some library headers and code. Used by all davros flavours
* scripts/ - scripts for starting openocd, minicom, gdb etc.
* testsuite/dv3 - a growing test suite for davros-3
* usb/ - a USB device stack
* LICENSE - a copy of GPL version 3
* CONTRIBUTING.md - how to contribute. tl;dr: send me a patch
* README.md - this file

## Hardware

### Raspberry pi zero

* Broadcom BCM2835 SoC based on an arm1176jzf-s core
* Broadcom interrupt controller and peripheral block bolted on the side

### Raspberry pi 2

Not supported.

* Broadcom BCM2836 SoC based on a quad-core Cortex A7 cluster
* Broadcom QA-7 block with interrupt controller, timers, mailboxes etc. bolted on the side
* Broadcom interrupt controller and peripheral block (same as the pi zero) bolted on the side of the QA-7

### Raspberry pi 3

* Broadcom BCM2836 SoC based on a quad-core Cortex A53 cluster
* Broadcom QA-7 block with interrupt controller, timers, mailboxes etc. bolted on the side
* Broadcom interrupt controller and peripheral block (same as the pi zero) bolted on the side of the QA-7

### Blue Pill

* STM32F103T8 microcontroller based on a Cortex M3 core

### Raspberry Pi Pico

* RP2040 microcontroller based on dual Cortex M0+ cores

The Cortex M0 and M0+ are Armv6-M architecure and have significant differences to the Armv7-M cores.
Among others:
* No baseprio register
* NVIC only has 32 IRQs
* No differentiation of exceptions - everything is hardfault

## Links

### Compilers

* <https://www.linaro.org/downloads/>
* <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>
* apt install gcc-arm-none-eabi binutils-arm-none-eabi

For the Raspberry Pi, the Linaro builds of gcc are probably the best. Grab the bare metal build - aarch64
for the Pi3 (in 64-bit mode), otherwise arm-eabi. You could probably use the linux build with a bit of tweaking.

For the Blue Pill (Cortex-M) the compiler from the arm-eabi build of linaro appears to work and the code
runs until the first setjmp(). It seems that the libraries don't support the Cortex-M's thumb mode properly. So
use the compiler from developer.arm.com. Unfortunately it seems to produce slightly bigger binaries.

For the Raspberry Pi Pico I use the installed gcc-arm-none-eabi from devuan. This compiler doesn't come with
a library that contains M0 versions of the low-level maths functions like __aeabi_uidiv. To supply the missing
functions I grabbed this one: git@github.com:bobbl/libaeabi-cortexm0.git. Tip: The division functions call
__aeabi_idiv0() if you try to divide by zero. The default version of that is in crt.S, which has a dependency
to main(). If you don't have a main() function, provide your own void __aeabi_idiv0(void) function. It might
be worthwhile to do that anyway and report an error rather than just return.

### Previous lives of davros

* <https://github.com/BackupTheBerlios/davros-kernel-svn>

Yes, it really has been around that long.

## Why the name "davros"?

I used to work for a company that manufactured work-time recording machines and access control systems.
The work-time machines went under the brand name of TimeLord. There was a machine for printing
the cards that the TimeLord used that looked remarkably like Doctor Who's robot dog. So much so,
in fact, that it got the nickname K9.

As part of my work there, I created a XINU-like operating system to run on Hitachi 6303 and similar processors.
It was during that time that I thought of the name davros, continuing the Doctor Who theme, but the OS
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
