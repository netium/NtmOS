#include "k_heap.h"

#define PAGE_SIZE (1024)
#define HEAP_BITMAP_SIZE (16384)
#define HEAP_SIZE (PAGE_SIZE * HEAP_BITMAP_SIZE)

static char* heap = ((char *)0x300000);
static int heap_alloc[HEAP_BITMAP_SIZE];

void* k_malloc(unsigned int size) {
    unsigned int real_size = size + 4;  // 4 bytes at front to store # of pages
    if (real_size >= HEAP_SIZE) return 0;

    unsigned int pages = real_size / PAGE_SIZE;
    pages += ((real_size % PAGE_SIZE) == 0 ? 0 : 1);

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
           char* p = heap + i * PAGE_SIZE;
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

    int alloc_index = ((char *)p - heap) / PAGE_SIZE;
    for (int i = alloc_index; i < alloc_index + pages; i++) {
        heap_alloc[i] = 0;
    }
}
