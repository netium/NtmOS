#ifndef _TSS_H_
#define _TSS_H_

#include <stddef.h>
#include "file.h"
#include "k_timer.h"
#include "kernel_inits.h"
#include "mm.h"
#include "segments.h"

// refer to: https://wiki.osdev.org/Task_State_Segment
typedef struct {
    uint16_t link, rev1;
    uint32_t esp0;
    uint16_t ss0, rev2;
    uint32_t esp1;
    uint16_t ss1, rev3;
    uint32_t esp2;
    uint16_t ss2, rev4;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es, rev5;
    uint16_t cs, rev6;
    uint16_t ss, rev7;
    uint16_t ds, rev8;
    uint16_t fs, rev9;
    uint16_t gs, rev10;
    uint16_t ldtr, rev11;
    uint16_t rev12, iopb_offset;
} tss_t;

typedef enum {
    TASK_UNUSED = 0x00,
    TASK_ALLOC  = 0x01,
    TASK_INIT   = 0x02,
    TASK_READY  = 0x03,
    TASK_RUNNING    = 0x04,
    TASK_BLOCK  = 0x05,
    TASK_ZOMBIE   = 0x06
} process_status_t;

typedef struct st_process_t process_t;

typedef struct st_process_t {
    uint32_t pid;   // process pid
    uint32_t tss_entry_id;  // tss pid in GDT
    process_status_t   status;  // process status
    ldt_entry_t ldt[2]; // offset: + 0x0c
    tss_t tss;  // offset: + 0x1c
    uint32_t umem_slot;
    simple_interrupt_event_queue_t event_queue;
    file_t * files[12];
    void * data_stack;        // base address of data stack segment
    uint32_t data_size;
    void * kern_stack;  // base address of kernel stack segment
    uint32_t kern_stack_size;
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

void init_process_management();

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
