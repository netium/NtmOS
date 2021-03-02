ASM = nasm
CC = gcc
DD = dd
# EDIMG = ./tools/edimg.exe

all: ntmos.img testapp

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

ntmio.sys: ntmio.sys.nas
	$(ASM) -fbin ntmio.sys.nas -o ntmio.sys

testapp: hlt.o boot_main.o test_app.c
	$(CC) -Wall -m32 test_app.c hlt.o boot_main.o -o testapp

boot_main.o: boot_main.c
	$(CC) -Wall -m32 boot_main.c -c -o boot_main.o

hlt.o: hlt.nas
	$(ASM) hlt.nas -f coff -o hlt.o

clean:
	rm *.sys *.img *.o testapp


