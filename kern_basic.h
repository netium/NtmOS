#ifndef _KERN_BASIC_H_
#define _KERN_BASIC_H_

#include <stddef.h>
#include <stdint.h>

// typedef unsigned int size_t;

#define KERN_BASE_VIR_ADDR ((size_t)0x80000000)
#define KERN_BASE_PHY_ADDR ((size_t)0x08000000)

#define IDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x0))
#define GDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x800))

#define N_GDT_ENTRIES 24
#define N_IDT_ENTRIES 256

#endif