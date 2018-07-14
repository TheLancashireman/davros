PRJ_ROOT        ?= $(shell pwd)
DV_ROOT         ?= $(PRJ_ROOT)/../../davros-3

# Select your board directory here
DV_BOARD_D      ?=  $(DV_ROOT)/board/arm64/raspberry-pi-3

# Select your compiler here
DV_COMPILER     ?=  gnu
DV_GNU_D        ?=  /data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf

ARMGNU ?= /data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf/bin/aarch64-elf

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -I. -I$(DV_BOARD_D)/h -I$(DV_ROOT)

gcc : uart05.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.srec


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart05.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -c uart05.c -o uart05.o

periph7.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph7.o

dv-arm-bcm2835-uart.o:	$(DV_ROOT)/devices/c/dv-arm-bcm2835-uart.c
	$(ARMGNU)-gcc $(COPS) -c $(DV_ROOT)/devices/c/dv-arm-bcm2835-uart.c -o dv-arm-bcm2835-uart.o

dv-arm-bcm2835-gpio.o:	$(DV_ROOT)/devices/c/dv-arm-bcm2835-gpio.c
	$(ARMGNU)-gcc $(COPS) -c $(DV_ROOT)/devices/c/dv-arm-bcm2835-gpio.c -o dv-arm-bcm2835-gpio.o

dv-kprintf.o:	$(DV_ROOT)/kernel/c/dv-kprintf.c
	$(ARMGNU)-gcc $(COPS) -c $(DV_ROOT)/kernel/c/dv-kprintf.c -o dv-kprintf.o

dv-xprintf.o:	$(DV_ROOT)/kernel/c/dv-xprintf.c
	$(ARMGNU)-gcc $(COPS) -c $(DV_ROOT)/kernel/c/dv-xprintf.c -o dv-xprintf.o

dv-strlen.o:	$(DV_ROOT)/kernel/c/dv-strlen.c
	$(ARMGNU)-gcc $(COPS) -c $(DV_ROOT)/kernel/c/dv-strlen.c -o dv-strlen.o

uart05.bin : memmap vectors.o uart05.o dv-arm-bcm2835-uart.o dv-arm-bcm2835-gpio.o dv-kprintf.o \
				dv-xprintf.o dv-strlen.o
	$(ARMGNU)-ld vectors.o uart05.o \
			dv-arm-bcm2835-uart.o dv-arm-bcm2835-gpio.o dv-kprintf.o dv-xprintf.o dv-strlen.o \
			-T memmap -o uart05.elf
	$(ARMGNU)-objdump -D uart05.elf > uart05.list
	$(ARMGNU)-objcopy uart05.elf -O ihex uart05.hex
	$(ARMGNU)-objcopy uart05.elf -O binary uart05.bin
	$(ARMGNU)-objcopy uart05.elf -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > uart05.srec


