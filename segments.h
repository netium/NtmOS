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
                // ac/rw/dc/ex form the 4bits segment type field
                unsigned int ac:1;
                unsigned int rw:1;
                unsigned int dc:1;
                unsigned int ex:1;
                unsigned int S:1;   // Descriptor type (0 = system; 1 = code or data)
                unsigned int DPL:2; // Descriptor privilege level
                unsigned int P:1;   // Segment present
            }; 
        } access;
        struct {
            int limit_high:4;
            int AVL:1;  // Available for use by system software
            int L:1;    // 64bit code segement (IA-32e mode only)
            int DB:1;   // Default operation size (0 = 16bit segement; 1 = 32bit segment)
            int G:1;    // Granularity
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