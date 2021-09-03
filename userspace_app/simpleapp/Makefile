ASM = nasm
LD = ld

# Use command 'objdump -D -Mintel,i386 -b binary -m i386 simpleapp.bin' to disassemble the file

.PHONY: all clean

all: simpleapp.bin

simpleapp.bin: simpleapp.o
	$(LD) simpleapp.o -o simpleapp.o.tmp -Ttext 0x400 -Map=simpleapp.map -m elf_i386
	objcopy -O binary -j.text -j.data -j.bss -j.rodata simpleapp.o.tmp simpleapp.bin

simpleapp.o: simpleapp.nas
	$(ASM) simpleapp.nas -felf32 -o simpleapp.o

clean:
	rm *.o *.bin *.tmp *.map