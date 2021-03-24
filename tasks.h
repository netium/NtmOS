#ifndef _TSS_H_
#define _TSS_H_

#include "k_timer.h"
#include "kernel_inits.h"

// refer to: https://wiki.osdev.org/Task_State_Segment
typedef struct {
    unsigned short link, rev1;
    unsigned int esp0;
    unsigned short ss0, rev2;
    unsigned int esp1;
    unsigned short ss1, rev3;
    unsigned int esp2;
    unsigned short ss2, rev4;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned short es, rev5;
    unsigned short cs, rev6;
    unsigned short ss, rev7;
    unsigned short ds, rev8;
    unsigned short fs, rev9;
    unsigned short gs, rev10;
    unsigned short ldtr, rev11;
    unsigned short rev12, iopb_offset;
} tss_t;

typedef enum {
    TASK_ALLOC  = 0x00,
    TASK_INIT   = 0x01,
    TASK_READY  = 0x02,
    TASK_RUNNING    = 0x03,
    TASK_BLOCK  = 0x04,
    TASK_EXIT   = 0x05
} task_status_t;

typedef struct st_task_t task_t;

struct st_task_t {
    unsigned long int task_id;
    task_status_t   status;
    simple_interrupt_event_queue_t event_queue;
    unsigned int tss_entry_id;  // tss id in GDT
    void * data;        // base address of data stack segment
    unsigned int data_size;
    void * kern_stack;  // base address of kernel stack segment
    unsigned int kern_stack_size;
    tss_t * tss;
    task_t *next;
    void * console; // No used now, but will used for determine which task takes the current activate console
};

extern tss_t g_tss3, g_tss4;

extern tss_t * current_task;

extern task_t * g_current_task;

extern task_t * g_ready_task_queue_head;
extern task_t ** g_ready_task_queue_tail_pointer;
extern task_t * g_sleep_task_queue_head;
extern task_t * g_finished_task_queue;
extern task_t * g_io_blocked_task_queue_head;

extern timer_t *g_task_switch_timer;

void task_main(task_t * task);

void switch_task(timer_t * timer);

task_t * task_alloc();

void task_free(task_t * task);

void task_init(task_t * task, int data_size, int kern_stack_size);

void start_task(task_t * task, void * start_addr);

void initial_task_event_queue(simple_interrupt_event_queue_t * event_queue);

task_t * initial_tasks();

#endif
