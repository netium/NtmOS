#ifndef _KERNEL_INITS_H_
#define _KERNEL_INITS_H_

#define IDT_TASK_GATE 0x5
#define IDT_INTERRUPT_GATE 0xe
#define IDT_TRAP_GATE 0xf

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

int mem_test();
void kernel_relocate();
void initial_gdt();
void initial_idt();

void initial_pic();

void initial_keyboard();

void set_interrupt(int interrupt_id, int code_seg_selector, void *p_handler, int gate_type, int priv_level, int enabled);

#endif