#ifndef _INTERRUPT_HANDLERS_H_
#define _INTERRUPT_HANDLERS_H_

typedef struct 
{
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    unsigned int esp;
    unsigned int ss;
} interrupt_frame_t;

void int00h_handler(interrupt_frame_t *frame, unsigned int error_code);
void int20h_handler(interrupt_frame_t *frame);
void int21h_handler(interrupt_frame_t *frame);
void int24h_handler(interrupt_frame_t *frame);
void int27h_handler(interrupt_frame_t *frame);
void int2ch_handler(interrupt_frame_t *frame);
void int40h_handler(interrupt_frame_t *frame);
#endif