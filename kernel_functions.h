#ifndef _KERNEL_FUNCTIONS_H_

#define _KERNEL_FUNCTIONS_H_

#include "kernel_types.h"

// All the inline GAS macro are written in the AT&T dialect instead of Intel dialect, this is because
// even though I am more comfortable and familiar with the Intel dialect, but looks like the GCC assembler
// has bugs then it will get error when compiling the 'LOCK INC" or "LOCK DEC" statement.

void _io_out8(int port, int data);
void _io_out16(int port, int data);
void _io_out32(int port, int data);

int _io_in8(int port);
int _io_in16(int port);
int _io_in32(int port);

int _get_eflags();
void _set_eflags(int eflags);

int _get_eip();

void _enable_interrupt_and_halt();

void _set_tr(int tr);

void _switch_task(int address, int tss_id);

void _panic();

void _putchar(int ch);

void _load_page_table(void *page_directory_addr);

void _jump_usermode(void * uip, void *usp);

#define cli() do { \
        __asm__ inline volatile ("cli"); \
    } while (0) 

#define sti() do { \
        __asm__ inline volatile ("sti"); \
    } while (0) 

#define sti_halt() do {\
        __asm__ inline volatile ("sti \n hlt"); \
    } while (0)

#define halt() do { \
        __asm__ inline volatile ("hlt \n "); \
    } while (0)

#define atom_inc(a) do { \
        __asm__ inline volatile ("lock incl %[v]":[v] "+m" (a)); \
    } while (0)

#define jump_task(a) do { \
        __asm__ inline volatile ("ljmp %[v]"::[v] "m" (a)); \
    } while (0)

#define load_gdt(a) do { \
        __asm__ inline volatile ("lgdt %[v]"::[v] "m" (a)); \
    } while (0)

#define load_idt(a) do { \
        __asm__ inline volatile ("lidt %[v]"::[v] "m" (a)); \
    } while (0)

#define load_ldt(a) do { \
        __asm__ inline volatile ("lldt %[v]"::[v] "m" (a)); \
    } while (0)

#define get_eflags(a) do { \
        __asm__ inline volatile ("pushfl\npopl %[v]":[v] "=rm" (a)); \
    } while (0)

#define set_eflags(a) do { \
        __asm__ inline volatile ("pushl %[v]\npopfl"::[v] "rm" (a)); \
    } while (0)

#define debug_halt(a) do { \
        __asm__ inline volatile ("movl %[v], %%eax\n1:loop\nhlt;\njmp 1b"::[v] "m"(a):"eax"); \
    } while(0)
#endif