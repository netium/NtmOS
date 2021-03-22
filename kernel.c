// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "kernel_inits.h"
#include "gui.h"
#include "k_heap.h"
#include "serial_port.h"
#include "tasks.h"

extern layer_t * bg_window;
extern layer_t * mouse_layer;

void kernel_main(void) {

	int mem_size = mem_test();

	kernel_relocate();

	k_printf("Kernel protected mode intialization start...");

	initial_interrupt_event_queue();

	k_printf("Init interrupt event queue complete!");

	initial_gdt();
	initial_idt();

	k_printf("Init GDT and LDT complete!");

	char str[256];

	// Initialize the Programmable Interrupt Controller
	initial_pic();

	k_printf("Init programmable interrupt controller complete!");

	_enable_interrupt();

	k_printf("Enable interrupt");

	initial_keyboard();

	k_printf("Init keyboard complete!");

	initial_pit();

	k_printf("Init prgrammable internal timer complete!");

	_io_out8(PIC0_IMR, 0xf8);	// enable PIT/PIC1/Keyboard

	int i = initial_serial(COM1_PORT);

	k_sprintf(str, "Init serial port COM1 with return value: %x", i);
	k_printf(str);

	// render_ui(bg_window);

	initial_mouse();

	_io_out8(PIC1_IMR, 0xef);	// enable mouse

	k_printf("Init mouse completed!");

	k_printf("Kernel is running......");

	initial_tasks();

	while (1) {
		_io_cli();	// Temporarily disable the interrupt, to prevent system from re-entry the manipulation of the event queue
		simple_interrupt_event_node_t *node = dequeue_event_queue();
		if (node == 0) {
			_enable_interrupt_and_halt();
		}
		else {
			_enable_interrupt();
			switch (node->type) {
				case KEYBOARD_EVENT:
					process_keyboard_event(&(node->keyboard_event));
					break;
				case MOUSE_EVENT:
					process_mouse_event(&(node->mouse_event));
					break;
				case TIMER_EVENT:
					process_timer_event(&(node->timer_event));
					break;
				default:
					break;
			}	
			k_free(node);
		}
	}

	// Done remove this statement, as it will casue the kernel main fucntion to return, 
	// and as this function is the entry point for kernel execution file, it will make let the function return back to an random address
	// and cause the virtual machine to complain about the crash.
	while(1) _io_hlt();

	return;
}