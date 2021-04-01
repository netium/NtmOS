#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>

typedef union {
    unsigned int dword;
    struct {
        size_t P:1;
        size_t R:1;
        size_t U:1;
        size_t W:1;
        size_t D:1;
        size_t A:1;
        size_t :1;   // Reserved
        size_t S:1;
        size_t G:1;
        size_t avail: 3;
        size_t addr: 20
    };
} page_directory_entry_t;


typedef union {
    unsigned int dword;
    struct {
        size_t P:1;
        size_t R:1;
        size_t U:1;
        size_t W:1;
        size_t C:1;
        size_t A:1;
        size_t D:1;   // Reserved
        size_t :1;
        size_t G:1;
        size_t avail: 3;
        size_t addr: 20
    }; 
} page_entry_t;

#endif