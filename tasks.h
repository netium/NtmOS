#ifndef _TSS_H_
#define _TSS_H_

#include <stddef.h>
#include "file.h"
#include "k_timer.h"
#include "kernel_inits.h"
#include "mm.h"

// refer to: https://wiki.osdev.org/Task_State_Segment
typedef struct {
    // __uint16_t link, rev1;
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
    TASK_ZOMBIE   = 0x05
} process_status_t;

typedef struct st_process_t process_t;

typedef struct st_process_t {
    long pid;   // process pid
    process_status_t   status;  // process status
    size_t umem_slot;
    tss_t tss;
    simple_interrupt_event_queue_t event_queue;
    unsigned int tss_entry_id;  // tss pid in GDT
    file_t * files[12];
    void * data_stack;        // base address of data stack segment
    size_t data_size;
    void * kern_stack;  // base address of kernel stack segment
    size_t kern_stack_size;
    process_t *next;
    void * console; // No used now, but will used for determine which task takes the current activate console
    long ppid;  // parent process pid
    page_directory_entry_t page_dir[1024];
} process_t ;

extern tss_t g_tss3, g_tss4;

extern process_t * g_current_task;

extern process_t * g_ready_task_queue_head;
extern process_t ** g_ready_task_queue_tail_pointer;
extern process_t * g_sleep_task_queue_head;
extern process_t * g_finished_task_queue;
extern process_t * g_io_blocked_task_queue_head;

extern timer_t *g_task_switch_timer;

void task_main(process_t * task);

void switch_task(timer_t * timer);

process_t * task_alloc();

void task_free(process_t * task);

void task_init(process_t * task, int data_size, int kern_stack_size);

void start_task(process_t * task, void * start_addr);

void initial_task_event_queue(simple_interrupt_event_queue_t * event_queue);

process_t * initial_tasks();

int enqueue_event_queue(simple_interrupt_event_queue_t *queue, simple_interrupt_event_node_t *p_node);

simple_interrupt_event_node_t * dequeue_event_queue(simple_interrupt_event_queue_t *queue);
#endif
