// asm(".code16gcc");

#include "kernel_functions.h"
#include "k_vga.h"
#include "gui.h"

void kernel_main(void) {
	unsigned char *p;
	p = (unsigned char*)0xa0000;

	init_palette();

	boxfill8(p, 320, COL8_FF0000, 20, 20, 120, 120);
	boxfill8(p, 320, COL8_00FF00, 70, 50, 170, 150);
	boxfill8(p, 320, COL8_0000FF, 120, 80, 220, 180);

	while (1) _io_hlt();

	return;
}
