#ifndef _KERNEL_TYPES_H_
#define _KERNEL_TYPES_H_
#include "kernel_types.h"

#pragma pack(1)
typedef union {
    unsigned int dwords[2];
    struct {
        unsigned short limit_low_word;
        unsigned short base_low_word;
        unsigned char base_mid_byte;
        union {
            unsigned char access_byte;
            struct {
                unsigned int ac:1;
                unsigned int rw:1;
                unsigned int dc:1;
                unsigned int ex:1;
                unsigned int access_resv:1;
                unsigned int privl:2;
                unsigned int pr:1;
            }; 
        } access;
        struct {
            int limit_high:4;
            union {
                int flags:4;
                struct {
                    int flag_resv1:1;
                    int flag_resv2:1;
                    int sz:1;
                    int gr:1;
                };
            };
        };
        unsigned char base_high_byte;
    } fields;
} gdt_entry_t;

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

#pragma pack(2)
typedef struct {
    unsigned short n_entries;
    idt_entry_t * p_start_addr;
} idtr_t;

#pragma pack(2)
typedef struct {
    unsigned short n_entries;
    gdt_entry_t * p_start_addr;
} gdtr_t;

#endif