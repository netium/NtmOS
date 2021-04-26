#ifndef _TTY_H_
#define _TTY_H_

#include "kernel_inits.h"

typedef struct st_tty_t {
    simple_interrupt_event_queue_t input_queue;
    int (*read)(struct st_tty_t *tty);
    int (*write)(struct st_tty_t *tty);
} tty_t;

extern tty_t tty[];

#endif