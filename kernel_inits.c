#include "kernel_inits.h"
#include "kernel_types.h"
#include "kernel_functions.h"

#define IDT_TABLE_START_ADDR  ((void *)0x0)
#define GDT_TABLE_START_ADDR  ((void *)0x800)
#define N_GDT_ENTRIES 24
#define N_IDT_ENTRIES 256

int mem_test() {
    return 0;
}

void kernel_relocate() {
    return;
}

void initial_gdt() {
    int eflags = _get_eflags();
    _disable_interrupt();

    gdt_entry_t *p_gdt_entries = GDT_TABLE_START_ADDR;

    p_gdt_entries[0].dwords[0] = 0x0;
    p_gdt_entries[0].dwords[1] = 0x0;

    p_gdt_entries[1].dwords[0] = 0x0000FFFF;
    p_gdt_entries[1].dwords[1] = 0x00CF9A00;

    p_gdt_entries[2].dwords[0] = 0x0000FFFF;
    p_gdt_entries[2].dwords[1] = 0x00CF9200;

    for (int i = 3; i < N_GDT_ENTRIES; i++) {
        p_gdt_entries[i].dwords[0] = 0x0;
        p_gdt_entries[i].dwords[1] = 0x0;
    }

    gdtr_t gdtr;
    gdtr.n_entries = N_GDT_ENTRIES;
    gdtr.p_start_addr = GDT_TABLE_START_ADDR;
    _load_gdt(gdtr);
    _set_eflags(eflags);
}

void initial_idt() {

    int eflags = _get_eflags();
    _disable_interrupt();

    idt_entry_t *p_idt_entries = IDT_TABLE_START_ADDR;

    for (int i = 0; i < N_IDT_ENTRIES; i++) {
        p_idt_entries[i].fields.offset_low_word =0;
        p_idt_entries[i].fields.offset_high_word = 0;
        p_idt_entries[i].fields.selector = 1;
        p_idt_entries[i].fields.zero = 0;
        p_idt_entries[i].fields.attr = 0;
    }

    idtr_t idtr;
    idtr.n_entries = N_IDT_ENTRIES;
    idtr.p_start_addr = IDT_TABLE_START_ADDR;
    _load_idt(idtr);
    _set_eflags(eflags);
}