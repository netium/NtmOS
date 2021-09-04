#ifndef _KERNEL_TYPES_H_
#define _KERNEL_TYPES_H_
#include "kernel_types.h"

#pragma pack(1)
typedef union {
    unsigned int dwords[2];
    struct {
        unsigned short offset_low_word;
        unsigned short selector;
        unsigned char zero;
        union {
            unsigned char attr;
            struct{
                unsigned int gate_type:4;
                unsigned int stor_seg:1;
                unsigned int desc_privl_level:2;
                unsigned int present:1;
            };
        };
        unsigned short offset_high_word;
    } fields;
} idt_entry_t;

#pragma pack(1)
typedef struct {
    unsigned short n_entries;
    idt_entry_t * p_start_addr;
} idtr_t;

#endif