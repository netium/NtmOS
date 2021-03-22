#ifndef _KERNEL_FUNCTIONS_H_

#define _KERNEL_FUNCTIONS_H_

#include "kernel_types.h"

void _io_hlt(void);
void _io_cli(void);

void _io_out8(int port, int data);
void _io_out16(int port, int data);
void _io_out32(int port, int data);

int _io_in8(int port);
int _io_in16(int port);
int _io_in32(int port);

int _get_eflags();
void _set_eflags(int eflags);

void _load_idt(idtr_t idtr);
void _load_gdt(gdtr_t gdtr);

int _get_eip();

void _disable_interrupt();

void _enable_interrupt();

void _enable_interrupt_and_halt();

void _set_tr(int tr);

void _switch_task(int address, int tss_id);

#endif

