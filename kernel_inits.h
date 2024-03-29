#ifndef _KERNEL_INITS_H_
#define _KERNEL_INITS_H_

#include "kern_basic.h" 
#include "k_timer.h"

typedef enum {
    IDT_TASK_GATE = 0x5,
    IDT_INTERRUPT_GATE = 0xe,
    IDT_TRAP_GATE = 0xf
} idt_gate_type_t;

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

#define PORT_KB_CMD     0x64
#define PORT_KB_DATA    0x60
#define PORT_KB_STATUS  0x64

typedef enum {
    KEYBOARD_EVENT = 0x1,
    MOUSE_EVENT = 0x2,
    TIMER_EVENT = 0x3
} event_type_t;

typedef struct {
    int data;
} keyboard_event_t;

typedef struct {
    int data;
} mouse_event_t;

typedef struct {
    timer_t *p_timer;
} timer_event_t;

typedef struct {
    int type;
    union {
        keyboard_event_t keyboard_event;
        mouse_event_t mouse_event;
        timer_event_t timer_event;
    };
} simple_interrupt_event_node_t;

typedef struct {
    unsigned int head;
    unsigned int tail;

    int full;

    simple_interrupt_event_node_t * nodes[1024];

} simple_interrupt_event_queue_t; 

void initial_interrupt_event_queue();

void initial_global_page_table();

void initial_idt();

void initial_pic();
void initial_pit();

void initial_keyboard();
void initial_mouse();

void set_interrupt(int interrupt_id, int code_seg_selector, void *p_handler, int gate_type, int priv_level, int enabled);

#endif