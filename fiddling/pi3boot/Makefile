PWD	:= $(shell pwd)

ifeq ($(PWD), /data1/projects/davros/fiddling/pi3boot)
TOOLDIR		= /data1/gnu
TFTPDIR		= /srv/tftp
else
TOOLDIR		= /home/dh/Work/Compiler/gnu/
TFTPDIR		= /home/dh/Work/tftp
endif

TOOLBASE	= $(TOOLDIR)/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
TOOLPRE		= aarch64-elf-
CC			= $(TOOLBASE)/bin/$(TOOLPRE)gcc
LD			= $(TOOLBASE)/bin/$(TOOLPRE)ld
OBJCOPY		= $(TOOLBASE)/bin/$(TOOLPRE)objcopy
CCOPT		= -Wall -fno-common

$(info TOOLDIR = $(TOOLDIR))
$(info TFTPDIR = $(TFTPDIR))
$(info TOOLBASE = $(TOOLBASE))
$(info TOOLPRE = $(TOOLPRE))
$(info CC = $(CC))
$(info LD = $(LD))
$(info OBJCOPY = $(OBJCOPY))
$(info CCOPT = $(CCOPT))

.PHONY:	default
default:	install

play.o:		play.c
	$(CC) $(CCOPT) -o play.o -c play.c

uart.o:		uart.c
	$(CC) $(CCOPT) -o uart.o -c uart.c

playAsm.o:	playAsm.S
	$(CC) $(CCOPT) -o playAsm.o -c playAsm.S

play.elf:	play.o uart.o playAsm.o play.ldscript
	$(LD) -o play.elf -T play.ldscript -e _start playAsm.o play.o uart.o

.PHONY: clean
clean:
	-rm play.abs play.elf *.o

.PHONY: install
install:	play.elf
	$(OBJCOPY) -Obinary --gap-fill 0 play.elf play.abs
