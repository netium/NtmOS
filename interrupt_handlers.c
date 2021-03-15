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

#include "gui.h"

__attribute__ ((interrupt)) void int21h_handler(interrupt_frame_t *frame) {
     unsigned char status;
     char keycode;

     _io_out8(0x20, 0x61);  // Write EOI (End of Interrupt) to PIC port to acknowledge

    unsigned char data = _io_in8(0x60);

    simple_interrupt_event_node_t *p_node = k_malloc(sizeof (simple_interrupt_event_node_t));

    if (p_node == 0) return;

    p_node->type = 0, p_node->keyboard_event.data = data;

    int ret = enqueue_event_queue(p_node);
}


__attribute__ ((interrupt)) void int2ch_handler(interrupt_frame_t *frame) {
    unsigned char data;

    _io_out8(PIC1_OCW2, 0x64);

    _io_out8(PIC0_OCW2, 0x62);

    data = _io_in8(0x60);

    simple_interrupt_event_node_t *p_node = k_malloc(sizeof (simple_interrupt_event_node_t));

    if (p_node == 0) return;

    p_node->type = 1, p_node->mouse_event.data = data;

    int ret = enqueue_event_queue(p_node);

    write_serial(COM1_PORT, data);
}

__attribute__ ((interrupt)) void int27h_handler(interrupt_frame_t *frame) {
    _io_out8(PIC0_OCW2, 0x67);
}