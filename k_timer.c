#include "k_timer.h"

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