# include "synchron.h"

inline int fetch_and_add(int* v, int by) {
    __asm__ volatile("lock; xaddl %0, %1"
        : "+r" (by), "+m" (*v)
        :
        : "memory"
    );
    return by;
}