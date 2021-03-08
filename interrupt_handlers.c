// Author: Bo Zhou (zb.public@outlook.com)
// Under GPL 2.0 License

// All functions in this file shall not be call directly as they are interrupt handlers
// There are several difference between the interrupt handlers and normal routines and one of the big
// diference is the interrupt handler uses IRET to return whereas the normal routine uses RET

#include "interrupt_handlers.h"

__attribute__ ((interrupt)) void int21h_handler(interrupt_frame_t *frame) {

}


__attribute__ ((interrupt)) void int2ch_handler(interrupt_frame_t *frame) {

}