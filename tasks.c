#include "tasks.h"
#include "gui.h"
#include "kernel_functions.h"
#include "interrupt_handlers.h"
#include "kernel_inits.h"
#include "k_heap.h"

// We will have a idle task, this task will always use the GDT slot 3;
// For other tasks, we will allocate the GDT slot dynamically

tss_t g_tss3, g_tss4;

tss_t * current_task = 0;

task_t * g_running_task = 0;
task_t * g_ready_task_queue_head = 0;
task_t * g_ready_task_queue_tail = 0;
task_t * g_sleep_task_queue_head = 0;
task_t * g_io_blocked_task_queue_head = 0;

timer_t * g_task_switch_timer = 0;

void task_main() {
    k_printf("A new task start!");

    _enable_interrupt();

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
}

void switch_task(timer_t * timer) {
    int eflags = _get_eflags();
    _io_cli();
    k_printf("Start to switch process...");
    k_set_timer_time(timer, 1000);
    if (current_task == &g_tss3) {
        k_printf("Switch to process 2");
        current_task = &g_tss4;
        _set_eflags(eflags);
        _switch_task(0, 4 << 3);
    }
    else if (current_task == &g_tss4) {
        k_printf("Switch to process 1");
        current_task = &g_tss3;
        _get_eflags(eflags);
        _switch_task(0, 3 << 3);
    }
}

task_t * task_alloc() {

}

void task_free(task_t * task) {

}

void task_init(task_t * task, int data_size, int kern_stack_size) {

}

void start_task(task_t * task, void * start_addr) {

}