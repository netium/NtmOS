# Author: Bo Zhou (zb.public@outlook.com)
# Under GPLv2.0 lincense

ASM = nasm
CC = gcc
DD = dd
LD = ld

CCFLAGS = -Wall -m32 -nolibc -nostdlib -nodefaultlibs -fno-pie -mmanual-endbr -fcf-protection=branch

# EDIMG = ./tools/edimg.exe

.PHONY: all clean

all: ntmos.img # testapp

ntmos.img: bootloader.img ntmio.sys kernel.sys
	# dd if=/dev/zero of=ntmos_new.img bs=512 count=2880
	# mkfs.vfat -F12 ntmos_new.img
	mv -f bootloader.img ntmos.img
	mcopy -i ntmos.img ntmio.sys ::/
	mcopy -i ntmos.img kernel.sys ::/

bootloader.img: bootloader.nas
	$(ASM) -fbin bootloader.nas -o bootloader.img 

ntmio.sys: ntmio.sys.o 
	# $(LD) ntmio.sys.o boot_main.o -e _start -m elf_i386 -o ntmio.sys.tmp --oformat binary --verbose -Ttext 0xc200
	# $(LD) ntmio.sys.o boot_main.o hlt.o -e _start -m elf_i386 -o ntmio.sys.tmp -Ttext 0xc200
	# mv ntmio.sys.tmp ntmio.sys
	# objcopy -O binary ntmio.sys.tmp ntmio.sys
	mv -f ntmio.sys.o ntmio.sys

ntmio.sys.o: ntmio.sys.nas
	$(ASM) -fbin ntmio.sys.nas -o ntmio.sys.o

kernel.sys: kernel_bootstrap.o kstring.o kernel.o kernel_inits.o kernel_functions.o k_vga.o gui.o interrupt_handlers.o k_heap.o serial_port.o k_timer.o tasks.o synchron.o keyboard.o mm.o
	$(LD) kernel_bootstrap.o kernel.o kernel_inits.o kernel_functions.o gui.o k_vga.o interrupt_handlers.o kstring.o k_heap.o serial_port.o k_timer.o tasks.o synchron.o keyboard.o mm.o -e kernel_bootstrap_main -m elf_i386 -o kernel.sys.tmp -Ttext 0x100000
	objcopy -O binary -j.text -j.data -j.bss -j.rodata kernel.sys.tmp kernel.sys

kernel_bootstrap.o: kernel_bootstrap.nas
	$(ASM) kernel_bootstrap.nas -felf32 -o kernel_bootstrap.o

testapp: hlt.o boot_main.o test_app.c
	$(CC) $(CCFLAGS) test_app.c hlt.o boot_main.o -o testapp

kernel.o: kernel.c 
	$(CC) $(CCFLAGS) kernel.c -c -o kernel.o

k_heap.o: k_heap.c k_heap.h
	$(CC) $(CCFLAGS) k_heap.c -c -o k_heap.o

kernel_inits.o: kernel_inits.c kernel_inits.h
	$(CC) $(CCFLAGS) kernel_inits.c -c -o kernel_inits.o

kernel_functions.o: kernel_functions.nas
	$(ASM) kernel_functions.nas -felf32 -o kernel_functions.o

gui.o: gui.c gui.h
	$(CC) $(CCFLAGS) gui.c -c -o gui.o

k_timer.o: k_timer.c k_timer.h
	$(CC) $(CCFLAGS) k_timer.c -c -o k_timer.o

k_vga.o: k_vga.c k_vga.h
	$(CC) $(CCFLAGS) k_vga.c -c -o k_vga.o

interrupt_handlers.o: interrupt_handlers.c interrupt_handlers.h
	$(CC) $(CCFLAGS) -mgeneral-regs-only -mno-red-zone interrupt_handlers.c -c -o interrupt_handlers.o

kstring.o: kstring.h kstring.c
	$(CC) $(CCFLAGS) kstring.c -c -o kstring.o

serial_port.o: serial_port.h serial_port.c
	$(CC) $(CCFLAGS) serial_port.c -c -o serial_port.o

tasks.o: tasks.h tasks.c
	$(CC) $(CCFLAGS) tasks.c -c -o tasks.o

synchron.o: synchron.h synchron.c
	$(CC) $(CCFLAGS) synchron.c -c -o synchron.o

keyboard.o: keyboard.h keyboard.c
	$(CC) $(CCFLAGS) keyboard.c -c -o keyboard.o

mm.o: mm.h mm.c
	$(CC) $(CCFLAGS) mm.c -c -o mm.o

clean:
	rm *.sys *.img *.o testapp

raw:
	objcopy -O binary testapp testapp.bin
