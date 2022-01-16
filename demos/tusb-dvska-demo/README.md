# TinyUSB + plain davroska demo

This demo implements a midi device.

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

The USB port is configured as a MIDI device.

Characters received from the serial port are translated to note commands and sent to the host.
Commands received from the host are displayed via the serial port.
