#ifndef _K_TIMER_H_
#define _K_TIMER_H_

#define MAX_TIMERS (0x256)

typedef enum {
    TIMER_ALLOC = 0x01,
    TIMER_USING = 0x02
} timer_flags_t;

typedef struct {
    unsigned long int timeout;
    unsigned int state;
    unsigned int flags;
    void (*pf)(unsigned int state, unsigned long int timeout);
} timer_t;

typedef struct {
    unsigned long int tick;
    timer_t timers[MAX_TIMERS]; 
} timer_control_t;

extern timer_control_t g_timer_control;

timer_t * k_timer_alloc();
void k_timer_free(timer_t * timer);

#endif