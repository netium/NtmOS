#ifndef _KERNEL_INITS_H_
#define _KERNEL_INITS_H_

#define IDT_TASK_GATE 0x5
#define IDT_INTERRUPT_GATE 0xe
#define IDT_TRAP_GATE 0xf

int mem_test();
void kernel_relocate();
void initial_gdt();
void initial_idt();

void initial_pic();

void set_interrupt(int interrupt_id, int code_seg_selector, void *p_handler, int gate_type, int priv_level, int enabled);

#endif