# TinyUSB + davroska-OSEK demo

This demo is inspired by the hid_composite_freertos example from the TinyUSB repository.

## Prerequisites

* TinyUSB - git@github.com:/hathach/tinyusb.git
* ST header files - git@github.com:STMicroelectronics/STM32CubeF1.git
* libaeabi-cortexm0 (for the pico) - git@github.com:bobbl/libaeabi-cortexm0.git

## What it does

The USB port is configured as a HID keyboard. Mouse, game controlled etc. from the TinyUSB example
are disabled.

The program sends a wise message ("All work and no play makes Jack a dull boy.") continuously,
so take care which window you have focussed when using the demo.

All the while, the same functionalty as the OSEK demo is running concurrently (erratic blinking
of onboard LED, simulated bank of 4 LEDs in serial terminal).

## Weaknesses

The message functionality uses the the HID_ASCII_TO_KEYCODE table from TinyUSB. This table
only considers the shift key, so many standard control characters will send a null keycode.
This isn't a problem for the default message, but don't rely on the table for your own applications.
