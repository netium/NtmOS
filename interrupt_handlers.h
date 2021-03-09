#ifndef _INTERRUPT_HANDLERS_H_
#define _INTERRUPT_HANDLERS_H_

typedef struct 
{
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    // uword_t sp;
    // uword_t ss;
} interrupt_frame_t;

void int21h_handler(interrupt_frame_t *frame);
void int2ch_handler(interrupt_frame_t *frame);
void int27h_handler(interrupt_frame_t *frame);

#endif