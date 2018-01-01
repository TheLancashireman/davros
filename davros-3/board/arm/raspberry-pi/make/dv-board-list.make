#	dv-board-list.make - board file lists for davros
#
#	Copyright 2015 David Haworth
#
#	This file is part of davros.
#
#	davros is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	davros is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with davros.  If not, see <http://www.gnu.org/licenses/>.

DV_LD_OBJS += $(DV_OBJ_D)/dv-raspberrypi-board.$(DV_O)
DV_LD_OBJS += $(DV_OBJ_D)/dv-arm-bcm2835-uart.$(DV_O)
DV_LD_OBJS += $(DV_OBJ_D)/dv-arm-bcm2835-gpio.$(DV_O)
DV_LD_OBJS += $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.$(DV_O)
DV_LD_OBJS += $(DV_OBJ_D)/dv-arm-bcm2835-timer.$(DV_O)
