#ifndef _KERN_BASIC_H_
#define _KERN_BASIC_H_

#include <stddef.h>
#include <stdint.h>

// typedef unsigned int size_t;

#define KERN_BASE_VIR_ADDR ((size_t)0x80000000)
#define KERN_BASE_PHY_ADDR ((size_t)0x08000000)

#define IDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x0))
#define GDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x10000))
#define GPT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x400000))
#define INIT_KERNEL_STACK_START_ADDR ((void *)(KERN_BASE_VIR_ADDR + 0x300000))
#define KERNEL_HEAP_START_ADDR ((void *)(KERN_BASE_VIR_ADDR + 0xC00000))

#define N_GDT_ENTRIES 24
#define N_IDT_ENTRIES 256

#endif