// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "kernel_inits.h"
#include "gui.h"

void kernel_main(void) {

	int mem_size = mem_test();

	kernel_relocate();

	init_screen();

	initial_gdt();
	initial_idt();

	// Initialize the Programmable Interrupt Controller
	initial_pic();

	_enable_interrupt();

	initial_keyboard();

	render_ui();

	/*
	while (1) {
		int key = _io_in8()
	}
	*/

	// Done remove this statement, as it will casue the kernel main fucntion to return, 
	// and as this function is the entry point for kernel execution file, it will make let the function return back to an random address
	// and cause the virtual machine to complain about the crash.
	while (1) _io_hlt();

	return;
}