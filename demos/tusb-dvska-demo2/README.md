# TinyUSB + plain davroska demo

This demo implements a USB communications device.

## Hardware

* Blue-pill STM32F103 - working
* Raspberry Pi (zero, 1, 2, 3, ...) - no support
* Raspberry pico
** work in progress, made difficult by the labrynthine nature of the Pico SDK :-(
** compiles and runs but doesn't register with host.

## Prerequisites

* TinyUSB - git@github.com:/hathach/tinyusb.git
* ST header files - git@github.com:STMicroelectronics/STM32CubeF1.git
* libaeabi-cortexm0 (for the pico) - git@github.com:bobbl/libaeabi-cortexm0.git

## What it does

The USB port is configured as a dual communications device. When conntect to a linux host,
it registers as two tty devices; e.g. /dev/ttyACMn
Open a minicom session on each of the devices. Whatever you type in one minicom session
appears in the other.
Note: the data transfer is raw, so to get a new line press CTRL-J
