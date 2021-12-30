#!/bin/sh
#
# Start OpenOCD to talk to the STM32 via ST-LINK v2
#

openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg
