#include "k_heap.h"
#include "kern_basic.h"

#define HEAP_PAGE_SIZE (512)
#define HEAP_BITMAP_SIZE (1024 * 1024)
#define HEAP_SIZE (HEAP_PAGE_SIZE * HEAP_BITMAP_SIZE)

// static char* heap = ((char *)(KERN_BASE_VIR_ADDR + 0x300000));
static char* heap_area_base_addr = (char *)KERNEL_HEAP_START_ADDR;
static int* heap_alloc = (int *)KERNEL_HEAP_START_ADDR;
static char* heap = (char *)KERNEL_HEAP_START_ADDR + sizeof(int) * HEAP_BITMAP_SIZE;

void* k_malloc(unsigned int size) {
    unsigned int real_size = size + 4;  // 4 bytes at front to store # of pages
    if (real_size >= HEAP_SIZE) return 0;

    unsigned int pages = real_size / HEAP_PAGE_SIZE;
    pages += ((real_size % HEAP_PAGE_SIZE) == 0 ? 0 : 1);

    int found;
    for (int i = 0; i < HEAP_BITMAP_SIZE - pages;) {
        for (int j = 0; j < pages; j++) {
            found = 1;
            if (heap_alloc[i+j] != 0) {
                found = 0;
                i += (j + 1);
                break;
            }
        }
        if (1 == found) {
           char* p = heap + i * HEAP_PAGE_SIZE;
           *(unsigned int *)p = pages; 
           p += sizeof(unsigned int);
           for (int j = 0; j < pages; j++) {
               heap_alloc[i+j] = 1;
           }
           return p;
        }
    }

    return 0;
}

void k_free(void *ptr) {
    unsigned int *p = ptr;
    p--;
    unsigned int pages = *p;
    if ((char *)p < heap) return;
    if (pages > HEAP_BITMAP_SIZE) return;

    int alloc_index = ((char *)p - heap) / HEAP_PAGE_SIZE;
    for (int i = alloc_index; i < alloc_index + pages; i++) {
        heap_alloc[i] = 0;
    }
}
