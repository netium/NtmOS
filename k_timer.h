#ifndef _K_TIMER_H_
#define _K_TIMER_H_

#define MAX_TIMERS (0x256)

typedef enum {
    TIMER_ALLOC = 0x01,
    TIMER_USING = 0x02
} timer_flags_t;

typedef struct timer_struct {
    unsigned long int timeout;
    unsigned int data;
    unsigned int flags;
    void (*pf)(struct timer_struct *timer);
    struct timer_struct *next;
} timer_t;

typedef struct {
    volatile unsigned long int tick;
    timer_t *next;
    timer_t timers[MAX_TIMERS]; 
} timer_control_t;

extern timer_control_t g_timer_control;

timer_t * k_timer_alloc();

void k_timer_free(timer_t * timer);

void k_init_timer(timer_t *timer, void (*p)(timer_t *), unsigned int data);

void k_set_timer_time (timer_t *timer, unsigned long int timeout);

#endif