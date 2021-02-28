NASM = nasm
EDIMG = ./tools/edimg.exe

all: ntmos.img

ntmos.img: bootloader.img ntmio.sys
	$(EDIMG)  imgin:./tools/fdimg0at.tek \
		wbinimg src:./bootloader.img len:512 from:0 to:0 \
		copy from:ntmio.sys to:@: \
		imgout:ntmos.img

bootloader.img: bootloader.nas
	$(NASM) bootloader.nas -o bootloader.img

ntmio.sys: ntmio.sys.nas
	$(NASM) ntmio.sys.nas -o ntmio.sys

clean:
	rm *.sys *.img


