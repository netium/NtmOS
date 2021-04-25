#include "mm.h"
#include "kernel_functions.h"

// We assume that the machine has a least physical 256MB RAM
// We will use from physical 64MB to 256MB RAM as the allocatable memory

#define LOW_MEM (0x4000000)
#define HIGH_MEM (0x10000000)
#define PAGE_SIZE (0x1000)      // 4096 byte per page
#define PAGE_SIZE_BITS (12)

#define N_PAGES (HIGH_MEM - LOW_MEM) / PAGE_SIZE

#define PAGE_INDEX(p_addr) ((p_addr - LOW_MEM) >> PAGE_SIZE_BITS)

static unsigned char page_frame_map[N_PAGES] = {0,};

unsigned long palloc() {
    for (int i = 0; i < N_PAGES; i++) {
        if (page_frame_map[i] == 0) {
            page_frame_map[i] == 1;
            return i << PAGE_SIZE_BITS;
        }
    }
    return 0;
}

void pfree(unsigned long addr) {
    if (addr < LOW_MEM) return;
    if (addr >= HIGH_MEM) _panic();
    addr = (addr - LOW_MEM) >> PAGE_SIZE_BITS;
    if (page_frame_map[addr]--) return;

    // If reach here, then means that it's trying to free a page that are not in used, this is an error
    page_frame_map[addr] = 0;
    _panic();
}

