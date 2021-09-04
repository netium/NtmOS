#include "segments.h"
#include "kernel_functions.h"


#define N_GDT_ENTRIES (8192)
#define N_LDT_ENTRIES (2)

void initial_gdt()
{
	gdt_entry_t *p_gdt_entries = GDT_TABLE_START_ADDR;

	p_gdt_entries[0].dwords[0] = 0x0;
	p_gdt_entries[0].dwords[1] = 0x0;

	p_gdt_entries[1].dwords[0] = 0x0000FFFF;
	p_gdt_entries[1].dwords[1] = 0x00CF9A00;

	p_gdt_entries[2].dwords[0] = 0x0000FFFF;
	p_gdt_entries[2].dwords[1] = 0x00CF9200;

	for (int i = 3; i < N_GDT_ENTRIES; i++)
	{
		p_gdt_entries[i].dwords[0] = 0x0;
		p_gdt_entries[i].dwords[1] = 0x0;
	}

	gdtr_t gdtr;
	gdtr.n_entries = (N_GDT_ENTRIES << 3) - 1;
	gdtr.p_start_addr = GDT_TABLE_START_ADDR;
	load_gdt(gdtr);
}

int install_tss_to_gdt(unsigned int slot, void *base_addr, unsigned int limit)
{
	if (slot == 0 || slot >= N_GDT_ENTRIES)
		_panic();	// Out of GDT boundary

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
int install_ldt_to_gdt(size_t slot, void *base_addr, size_t limit) {
	if (slot == 0 || slot >= N_GDT_ENTRIES)
		_panic();	// Out of GDT boundary

	gdt_entry_t *entry = ((gdt_entry_t *)GDT_TABLE_START_ADDR) + slot;
	entry->fields.base_low_word = (size_t)base_addr & 0xFFFF;
	entry->fields.base_mid_byte = ((size_t)base_addr >> 16) & 0xFF;
	entry->fields.base_high_byte = ((size_t)base_addr >> 24) & 0xFF;
	entry->fields.limit_low_word = (size_t)limit & 0xFFFF;
	entry->fields.limit_high = ((size_t)limit >> 16) & 0xF;
	entry->fields.access.access_byte = 0x89;
	entry->fields.access.privl = 0x3;
	entry->fields.gr = 0x0;
	entry->fields.sz = 0x1;
}

int uninstall_gdt_entry(size_t slot)
{
	if (slot == 0 || slot >= N_GDT_ENTRIES)
		_panic();	// Out of GDT boundary

	gdt_entry_t *entry = ((gdt_entry_t *)GDT_TABLE_START_ADDR) + slot;
	entry->fields.access.pr = 0; // Simply set present bit to 0 to mark as unpresent

	return 0;
}

int install_seg_to_ldt(void *ldt_addr, size_t slot, void *base_addr, size_t limit) {
	if (slot >= N_LDT_ENTRIES)
		_panic();

	ldt_entry_t * entry = ((ldt_entry_t *)ldt_addr + slot);
	entry->fields.base_low_word = (size_t)base_addr & 0xFFFF;
	entry->fields.base_mid_byte = ((size_t)base_addr >> 16) & 0xFF;
	entry->fields.base_high_byte = ((size_t)base_addr >> 24) & 0xFF;
	entry->fields.limit_low_word = (size_t)limit & 0xFFFF;
	entry->fields.limit_high = ((size_t)limit >> 16) & 0xF;
	entry->fields.access.access_byte = 0x89;
	entry->fields.access.privl = 0x3;
	entry->fields.gr = 0x0;
	entry->fields.sz = 0x1;
}