#ifndef _MM_H_
#define _MM_H_

#include "kern_basic.h"

#define N_PAGE_DIRECTORY_ENTRIES (1024)
#define N_PAGE_TABLE_ENTRIES (1024)
#define PAGE_SIZE (0x1000)      // 4096 byte per page
#define PAGE_SIZE_BITS (12)

typedef union {
    uint32_t dword;
    struct {
        uint32_t P:1;
        uint32_t R:1;
        uint32_t U:1;
        uint32_t W:1;
        uint32_t D:1;
        uint32_t A:1;
        uint32_t :1;   // Reserved
        uint32_t S:1;
        uint32_t G:1;
        uint32_t avail: 3;
        uint32_t addr: 20;
    };
} page_directory_entry_t;


typedef union {
    uint32_t dword;
    struct {
        uint32_t P:1;
        uint32_t R:1;
        uint32_t U:1;
        uint32_t W:1;
        uint32_t C:1;
        uint32_t A:1;
        uint32_t D:1;   // Reserved
        uint32_t :1;
        uint32_t G:1;
        uint32_t avail: 3;
        uint32_t addr: 20;
    }; 
} page_entry_t;

unsigned long palloc();
void pfree(unsigned long addr);

int proc_umem_alloc();
void proc_umem_free();
size_t get_proc_umem_start_address(int i);
size_t get_proc_umem_end_address(int i);

#endif