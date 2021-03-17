#include "k_timer.h"
#include "kernel_functions.h"

timer_control_t g_timer_control;

timer_t * k_timer_alloc() {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (g_timer_control.timers[i].flags == 0) {
            g_timer_control.timers[i].flags |= TIMER_ALLOC;
            return &g_timer_control.timers[i];
        }
    }
    return 0;
}

void k_timer_free(timer_t * timer) {
    if (timer < &g_timer_control.timers[0] || timer >= &g_timer_control.timers + MAX_TIMERS) return;

    timer->flags = 0;
}

void k_init_timer(timer_t *timer, void (*p)(timer_t *), unsigned int data) {
    if (0 == timer) return;
    timer->data = data;
    timer->pf = p;
}

void k_set_timer_time (timer_t *timer, unsigned long int timeout) {
    if (0 == timer) return;

    int e;

    timer->timeout = timeout + g_timer_control.tick;
    timer->flags = TIMER_USING;

    e = _get_eflags();
    _io_cli();

    timer_t ** pp_next = &g_timer_control.next;

    while (*pp_next != 0 && (*pp_next)->timeout < timer->timeout) pp_next = &(*pp_next)->next;

    timer->next = *pp_next;
    *pp_next = timer;

    _set_eflags(e); 
}