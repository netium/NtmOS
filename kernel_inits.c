#include "kernel_inits.h"
#include "kernel_types.h"
#include "kernel_functions.h"
#include "interrupt_handlers.h"

#define IDT_TABLE_START_ADDR  ((void *)0x0)
#define GDT_TABLE_START_ADDR  ((void *)0x800)
#define N_GDT_ENTRIES 24
#define N_IDT_ENTRIES 256

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

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

    set_interrupt(0x21, 0x01, int21h_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x2c, 0x01, int2ch_handler, IDT_INTERRUPT_GATE, 0x0, 1);
}

void initial_pic() {
    _io_out8(PIC0_IMR, 0xff);
    _io_out8(PIC1_IMR, 0xff);
    _io_out8(PIC0_ICW1, 0x11);
    _io_out8(PIC0_ICW2, 0x20);
    _io_out8(PIC0_ICW3, 1 << 2);
    _io_out8(PIC0_ICW4, 0x01);
    _io_out8(PIC1_ICW1, 0x11);
    _io_out8(PIC1_ICW2, 0x28);
    _io_out8(PIC1_ICW3, 0x02);
    _io_out8(PIC1_ICW4, 0x01);
    _io_out8(PIC0_IMR, 0xfb);
    _io_out8(PIC1_IMR, 0xff);
}

void set_interrupt(int interrupt_id, int code_seg_selector, void *p_handler, int gate_type, int priv_level, int enabled) {
    if (interrupt_id >= 256) return;
    if (priv_level >= 4) return;
    if (p_handler == 0) enabled = 0;

    int eflags = _get_eflags();

    // _io_cli();

    idt_entry_t *p_idt = 0x0;

    p_idt[interrupt_id].fields.offset_low_word = (unsigned int)(p_handler) & 0xFFFF;
    p_idt[interrupt_id].fields.offset_high_word = ((unsigned int)(p_handler) >> 16) & 0xFFFF;

    p_idt[interrupt_id].fields.gate_type = gate_type;
    p_idt[interrupt_id].fields.desc_privl_level = priv_level;

    p_idt[interrupt_id].fields.stor_seg = 0;

    p_idt[interrupt_id].fields.present = enabled != 0 ? 1 : 0;

    p_idt[interrupt_id].fields.selector = code_seg_selector; 

    _set_eflags(eflags);
}