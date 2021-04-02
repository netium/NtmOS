#include "kern_basic.h"
#include "kernel_inits.h"
#include "kernel_types.h"
#include "kernel_functions.h"
#include "interrupt_handlers.h"
#include "kstring.h"
#include "gui.h"
#include "k_heap.h"
#include "serial_port.h"
#include "k_timer.h"
#include "tasks.h"
#define IDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x0))
#define GDT_TABLE_START_ADDR  ((void *)(KERN_BASE_VIR_ADDR + 0x800))
#define N_GDT_ENTRIES 24
#define N_IDT_ENTRIES 256

static unsigned char mouse_buf[3];
static int mouse_dx = 0, mouse_dy = 0, mouse_button = 0;

static int raw_mouse_state_machine = 0;

void wait_keyboard_send_ready(void);

int mem_test() {
    return 0;
}

void kernel_relocate() {
    return;
}

void initial_gdt() {
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
    gdtr.n_entries = (N_GDT_ENTRIES << 3) - 1;
    gdtr.p_start_addr = GDT_TABLE_START_ADDR;
    _load_gdt(gdtr);
}

int set_tss_into_gdt(unsigned int slot, void *base_addr, unsigned int limit) {
    if (slot >= 8192) return -1;

    gdt_entry_t *entry = ((gdt_entry_t *)GDT_TABLE_START_ADDR) + slot;
    entry->fields.base_low_word = (unsigned int)base_addr & 0xFFFF;
    entry->fields.base_mid_byte = ((unsigned int)base_addr >> 16) & 0xFF;
    entry->fields.base_high_byte = ((unsigned int)base_addr >> 24) & 0xFF;
    entry->fields.limit_low_word = (unsigned int)limit & 0xFFFF;
    entry->fields.limit_high = ((unsigned int)limit >> 16) & 0xF; 
    entry->fields.access.access_byte = 0x89;
    entry->fields.gr = 0x0;
    entry->fields.sz = 0x1;

    return 0;
}

void initial_idt() {

    idt_entry_t *p_idt_entries = IDT_TABLE_START_ADDR;

    for (int i = 0; i < N_IDT_ENTRIES; i++) {
        p_idt_entries[i].fields.offset_low_word =0;
        p_idt_entries[i].fields.offset_high_word = 0;
        p_idt_entries[i].fields.selector = 0;
        p_idt_entries[i].fields.zero = 0;
        p_idt_entries[i].fields.attr = 0;
    }

    set_interrupt(0x00, 0x01, int00h_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x20, 0x01, int20h_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x21, 0x01, int21h_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x2c, 0x01, int2ch_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x27, 0x01, int27h_handler, IDT_INTERRUPT_GATE, 0x0, 1);
    set_interrupt(0x40, 0x01, int40h_handler, IDT_INTERRUPT_GATE, 0x0, 1);

    idtr_t idtr;
    idtr.n_entries = N_IDT_ENTRIES << 3;
    idtr.p_start_addr = IDT_TABLE_START_ADDR;
    _load_idt(idtr);

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

   // Refer to: https://arjunsreedharan.org/post/99370248137/kernels-201-lets-write-a-kernel-with-keyboard
	/* ICW1 - begin initialization */
    // _io_out8(0x20 , 0x11);
	// _io_out8(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	// _io_out8(0x21 , 0x20);
	// _io_out8(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	// _io_out8(0x21 , 0x00);  
	// _io_out8(0xA1 , 0x00);  

	/* ICW4 - environment info */
	// _io_out8(0x21 , 0x01);
	// _io_out8(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	// _io_out8(0x21 , 0xff);
	// _io_out8(0xA1 , 0xff);
}

void initial_pit() {
    _io_out8(0x43, 0x34);
    _io_out8(0x40, 0x9c);
    _io_out8(0x40, 0x2e);

    g_timer_control.tick = 0x0L;

    for (int i = 0; i < MAX_TIMERS; i++) {
        g_timer_control.timers->flags = 0;
    }
}

void initial_keyboard() {
    /*
    wait_keyboard_send_ready();
    _io_out8(0x21, 0xfd);
    wait_keyboard_send_ready();
    _io_out8(0xa1, 0xff);

    wait_keyboard_send_ready();
    _io_out8(PIC0_IMR, 0xf9);
    wait_keyboard_send_ready();
    _io_out8(PIC1_IMR, 0xef);
    */

   _io_out8(PIC0_IMR, 0xf9);
   _io_out8(PIC1_IMR, 0xef);

   wait_keyboard_send_ready();
   _io_out8(PORT_KB_CMD, 0x60);
   wait_keyboard_send_ready();
   _io_out8(PORT_KB_DATA, 0x47);

}

void initial_mouse() {
    wait_keyboard_send_ready();
    _io_out8(PORT_KB_CMD, 0xd4);

    wait_keyboard_send_ready();
    _io_out8(PORT_KB_DATA, 0xf4);
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

    p_idt[interrupt_id].fields.selector = code_seg_selector << 3; 

    _set_eflags(eflags);
}

void wait_keyboard_send_ready(void) {
    while (1) {
        if ((_io_in8(PORT_KB_STATUS) & 0x02) == 0) {
            break;
        }
    }
}