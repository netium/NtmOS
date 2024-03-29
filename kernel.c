// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "segments.h"
#include "kernel_inits.h"
#include "gui.h"
#include "k_heap.h"
#include "serial_port.h"
#include "tasks.h"
#include "keyboard.h"
#include "synchron.h"
#include "harddisk.h"
#include "fs.h"

/*
extern layer_t * bg_window;
extern layer_t * mouse_layer;
*/

void kernel_main(void) {

	cli();

	// Temporarity comment out the following function as the kernel bootstrap code didn't create the identity mapping for the whole 0-256MB RAM space.
	// initial_global_page_table();

	k_printf("Kernel protected mode intialization start...");

	initial_gdt();
	initial_idt();

	k_printf("Init GDT and LDT complete!");

	char str[256];

	// Initialize the Programmable Interrupt Controller
	initial_pic();

	k_printf("Init programmable interrupt controller complete!");

	initial_keyboard();

	k_printf("Init keyboard complete!");

	initial_pit();

	k_printf("Init prgrammable internal timer complete!");

	_io_out8(PIC0_IMR, 0xf8);	// enable PIT/PIC1/Keyboard


	int i = initial_serial(COM1_PORT);

	k_sprintf(str, "Init serial port COM1 with return value: %x", i);
	k_printf(str);

	initial_mouse();

	_io_out8(PIC1_IMR, 0xef);	// enable mouse

	k_printf("Init mouse completed!");

	init_harddisks();
	k_printf("Init HDD completed!");

	int ret = init_root_filesystem();
	if (ret == 0)
		k_printf("Root filesystem initialization complete!");
	else
		k_printf("Root filesystem initialization failed!");

	init_process_management();

	k_printf("Init process management subsystem completed");

	k_printf("Kernel is running......");

	process_t * current_task = initial_tasks();

	process_t * new_task = task_alloc();

	task_init(new_task, 8192, 8192 * 2);

	start_task(new_task, task_main);

	k_sprintf(str, "New task %x added", (unsigned int)new_task);
	k_printf(str);

	kern_exec(current_task);
	
	task_main(current_task);

	// Done remove this statement, as it will casue the kernel main fucntion to return, 
	// and as this function is the entry point for kernel execution file, it will make let the function return back to an random address
	// and cause the virtual machine to complain about the crash.
	while(1) halt();

	k_printf("kernel end");
	_panic();

	return;
}

