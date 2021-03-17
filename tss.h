#ifndef _TSS_H_
#define _TSS_H_

// refer to: https://wiki.osdev.org/Task_State_Segment
typedef struct {
    unsigned short link, rev1;
    unsigned int esp0;
    unsigned short ss0, rev2;
    unsigned int esp1;
    unsigned short ss1, rev3;
    unsigned int esp2;
    unsigned short ss2, rev4;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned short es, rev5;
    unsigned short cs, rev6;
    unsigned short ss, rev7;
    unsigned short ds, rev8;
    unsigned short fs, rev9;
    unsigned short gs, rev10;
    unsigned short ldtr, rev11;
    unsigned short rev12, iopb_offset;
} tss_t;

#endif
