// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "kernel_inits.h"
#include "gui.h"

void kernel_main(void) {

	int mem_size = mem_test();

	kernel_relocate();

	initial_gdt();
	initial_idt();

	render_ui();

	while (1) _io_hlt();

	return;
}
