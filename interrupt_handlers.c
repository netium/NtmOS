// Author: Bo Zhou (zb.public@outlook.com)
// Under GPL 2.0 License

// All functions in this file shall not be call directly as they are interrupt handlers
// There are several difference between the interrupt handlers and normal routines and one of the big
// diference is the interrupt handler uses IRET to return whereas the normal routine uses RET

#include "interrupt_handlers.h"
#include "kernel_functions.h"
#include "kernel_inits.h"

#include "gui.h"

__attribute__ ((interrupt)) void int21h_handler(interrupt_frame_t *frame) {
     boxfill8_ds(320, COL8_FF0000, 30, 30, 50, 50);
}


__attribute__ ((interrupt)) void int2ch_handler(interrupt_frame_t *frame) {
    boxfill8_ds(320, COL8_00FF00, 30, 30, 50, 50);
}

__attribute__ ((interrupt)) void int27h_handler(interrupt_frame_t *frame) {
    _io_out8(PIC0_OCW2, 0x67);
}