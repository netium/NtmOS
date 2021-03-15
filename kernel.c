// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "kernel_inits.h"
#include "gui.h"
#include "k_heap.h"
#include "serial_port.h"

extern layer_t * bg_window;
extern layer_t * mouse_layer;

void kernel_main(void) {

	int mem_size = mem_test();

	kernel_relocate();

	init_screen();

	initial_interrupt_event_queue();

	initial_gdt();
	initial_idt();

	// Initialize the Programmable Interrupt Controller
	initial_pic();

	_enable_interrupt();

	initial_keyboard();

	int i = initial_serial(COM1_PORT);

	draw_uint_hex_scr(0, 8, COL8_00FFFF, i);

	render_ui(bg_window);

	initial_mouse();

	// Done remove this statement, as it will casue the kernel main fucntion to return, 
	// and as this function is the entry point for kernel execution file, it will make let the function return back to an random address
	// and cause the virtual machine to complain about the crash.
	while (1) {
		_io_cli();	// Temporarily disable the interrupt, to prevent system from re-entry the manipulation of the event queue
		simple_interrupt_event_node_t *node = dequeue_event_queue();
		if (node == 0) {
			_enable_interrupt_and_halt();
		}
		else {
			_enable_interrupt();
			switch (node->type) {
				case 0:
					process_keyboard_event(&(node->keyboard_event));
					break;
				case 1:
					process_mouse_event(&(node->mouse_event));
					break;
				default:
					break;
			}	
			k_free(node);
		}
	}

	while(1) _io_hlt();

	return;
}

