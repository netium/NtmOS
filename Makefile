ASM = nasm
CC = gcc
DD = dd
LD = ld

CCFLAGS = -Wall -m16

# EDIMG = ./tools/edimg.exe

.PHONY: all clean

all: ntmos.img # testapp

ntmos.img: bootloader.img ntmio.sys
	dd if=/dev/zero of=ntmos_new.img bs=512 count=2880
	mkfs.vfat -F12 ntmos_new.img
	#dd if=./bootloader.img of=ntmos.img bs=512 count=1
	mv -f bootloader.img ntmos.img
	mcopy -i ntmos.img ntmio.sys ::/
# ntmos.img: bootloader.img ntmio.sys
#	$(EDIMG)  imgin:./tools/fdimg0at.tek \
#		wbinimg src:./bootloader.img len:512 from:0 to:0 \
#		copy from:ntmio.sys to:@: \
#		imgout:ntmos.img

bootloader.img: bootloader.nas
	$(ASM) -fbin bootloader.nas -o bootloader.img 

ntmio.sys: ntmio.sys.o boot_main.o	
	# $(LD) ntmio.sys.o boot_main.o -e _start -m elf_i386 -o ntmio.sys.tmp --oformat binary --verbose -Ttext 0xc200
	$(LD) ntmio.sys.o boot_main.o -e _start -m elf_i386 -o ntmio.sys.tmp --verbose -Ttext 0xc200
	# mv ntmio.sys.tmp ntmio.sys
	objcopy -O binary ntmio.sys.tmp ntmio.sys

ntmio.sys.o: ntmio.sys.nas
	$(ASM) -felf32 ntmio.sys.nas -o ntmio.sys.o

testapp: hlt.o boot_main.o test_app.c
	$(CC) $(CCFLAGS) test_app.c hlt.o boot_main.o -o testapp

boot_main.o: boot_main.c
	$(CC) $(CCFLAGS) -nolibc -nostdlib -nodefaultlibs -fno-pie boot_main.c -mmanual-endbr -fcf-protection=branch -c -o boot_main.o

hlt.o: hlt.nas
	$(ASM) hlt.nas -f coff -o hlt.o

clean:
	rm *.sys *.img *.o testapp

raw:
	objcopy -O binary testapp testapp.bin
