#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "kern_basic.h"

typedef enum {
    KEY_DOWN = 0x0,
    KEY_UP = 0x1
} keypress_t;

typedef struct {
    int32_t keychar;
    keypress_t keypress; 
} keycode_t;

extern const keycode_t g_keyboard_scancode_set_1[];

extern const int g_keyboard_scancode_set_1_size;

#endif