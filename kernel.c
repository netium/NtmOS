// asm(".code16gcc");

#include "kernel_functions.h"
#include "k_vga.h"

void kernel_main(void) {
	char *p;
	p = (char*)0xa0000;

	init_palette();

	for (int i = 0; i < 0xc0000; i++) {
		p[i] = i & 0x0f; 
	}

	while (1) _io_hlt();

	return;
}
