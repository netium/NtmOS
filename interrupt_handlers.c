// Author: Bo Zhou (zb.public@outlook.com)
// Under GPL 2.0 License

// All functions in this file shall not be call directly as they are interrupt handlers
// There are several difference between the interrupt handlers and normal routines and one of the big
// diference is the interrupt handler uses IRET to return whereas the normal routine uses RET

#include "interrupt_handlers.h"
#include "kernel_functions.h"
#include "kernel_inits.h"
#include "k_heap.h"
#include "serial_port.h"
#include "tasks.h"

#include "gui.h"

#include "k_timer.h"

#include "keyboard.h"

__attribute__ ((interrupt)) void int00h_handler(interrupt_frame_t *frame, unsigned int error_code) {

}

__attribute__ ((interrupt)) void int0dh_handler(interrupt_frame_t *frame, unsigned int error_code) {
    k_printf("#GP Exception occur");
}

// Programmable internal timer interrrupt
__attribute__ ((interrupt)) void int20h_handler(interrupt_frame_t *frame) {
    _io_out8(PIC0_OCW2, 0x60);

    g_timer_control.tick++;

    if (g_timer_control.next == 0 || g_timer_control.next->timeout > g_timer_control.tick) return;

    timer_t ** pp_next = &g_timer_control.next;

    int time_to_switch_task = 0;

    while (*pp_next != 0 && (*pp_next)->timeout <= g_timer_control.tick) {
        timer_t *timer = *pp_next;
        *pp_next = timer->next;
        timer->next = 0;
        timer->flags = TIMER_ALLOC;

        // If the timer is for task switch, then don't put it into the task' event queue
        // But directly handle by the kernel
        if (timer == g_task_switch_timer) {
            time_to_switch_task = 1;
        }
        else {

            simple_interrupt_event_node_t *p_node = k_malloc(sizeof (simple_interrupt_event_node_t));

            if (p_node == 0) {
                k_printf("Cannot allocate event node");
                break;
            }

            p_node->type = TIMER_EVENT, p_node->timer_event.p_timer = timer;

            int ret = enqueue_event_queue(&g_current_task->event_queue, p_node);
        }
    }

    if (1 == time_to_switch_task) {
        sti();
        g_task_switch_timer->pf(g_task_switch_timer);
    }
}

// Keyboard interrupt
__attribute__ ((interrupt)) void int21h_handler(interrupt_frame_t *frame) {
    
     unsigned char status;
     char keycode;

     _io_out8(PIC0_OCW2, 0x61);  // Write EOI (End of Interrupt) to PIC port to acknowledge

    unsigned char data = _io_in8(0x60);

    unsigned char ch = (data <= 0xD8 ? g_keyboard_scancode_set_1[data].keychar : data);

    simple_interrupt_event_node_t *p_node = k_malloc(sizeof (simple_interrupt_event_node_t));

    if (p_node == 0) return;

    p_node->type = KEYBOARD_EVENT, p_node->keyboard_event.data = ch;

    int ret = enqueue_event_queue(&g_current_task->event_queue, p_node);
}

// COM ports interrupt
__attribute__ ((interrupt)) void int24h_handler(interrupt_frame_t *frame) {
    // This is for COM ports
}

// Mouse interrupt
__attribute__ ((interrupt)) void int2ch_handler(interrupt_frame_t *frame) {
    unsigned char data;

    _io_out8(PIC1_OCW2, 0x64);

    _io_out8(PIC0_OCW2, 0x62);

    data = _io_in8(0x60);

    simple_interrupt_event_node_t *p_node = k_malloc(sizeof (simple_interrupt_event_node_t));

    if (p_node == 0) return;

    p_node->type = MOUSE_EVENT, p_node->mouse_event.data = data;

    int ret = enqueue_event_queue(&g_current_task->event_queue, p_node);

    write_serial(COM1_PORT, data);
}

__attribute__ ((interrupt)) void int27h_handler(interrupt_frame_t *frame) {
    _io_out8(PIC0_OCW2, 0x67);
}

__attribute__ ((interrupt)) void int80h_handler(interrupt_frame_t *frame) {
    cli();
    char buf[256];
    k_printf("System call interrupt triggered");
    k_sprintf(buf, "The interrupt frame is located at %x.", frame);
    k_printf(buf);
    halt();
    sti();    // It's for system call, so can enable interrupt
}