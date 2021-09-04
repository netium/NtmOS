#ifndef _SEGMENTS_H_
#define _SEGMENTS_H_

#include "kern_basic.h"
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
            int flag_resv1:1;
            int flag_resv2:1;
            int sz:1;
            int gr:1;
        };
        unsigned char base_high_byte;
    } fields;
} gdt_entry_t, ldt_entry_t;

#pragma pack(1)
typedef struct {
    unsigned short n_entries;
    gdt_entry_t * p_start_addr;
} gdtr_t;

void initial_gdt();

int install_ldt_to_gdt(size_t slot, void *base_addr, size_t limit);

int install_tss_to_gdt(size_t slot, void *base_addr, size_t limit);

int uninstall_gdt_entry(size_t slot);

int install_seg_to_ldt(void *ldt_addr, size_t slot, void *base_addr, size_t limit);

#endif