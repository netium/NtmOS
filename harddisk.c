#include "kern_basic.h"
#include "kernel_functions.h"
#include "harddisk.h"

void init_harddisks() {
	uint8_t buff[512];
	ata_lba_read(buff, 0, 1);	

	char buf[256];
	for (int i = 0; i < 10; i++) {
		k_sprintf(buf, "hdd[%x] read: [%x]", i, buf[i]);
		k_printf(buf);
	}
}

// Implementation refer to https://wiki.osdev.org/ATA_read/write_sectors

int ata_lba_read(uint8_t * buff, size_t sector_lba, size_t num_sectors) {
	uint16_t * buffw = (uint16_t *)buff;
	sector_lba &= 0x0fffffff;

	_io_out8(0x01f6, (uint8_t)((sector_lba >> 24) | 0xe0));	// port to send drive and bit 24-27 of LBA, also set bit 6 for LBA mode

	_io_out8(0x01f2, (uint8_t)num_sectors);	// send number of sectors

	_io_out8(0x01f3, (uint8_t)sector_lba);

	_io_out8(0x01f4, (uint8_t)(sector_lba >> 8));

	_io_out8(0x01f5, (uint8_t)(sector_lba >> 16));

	_io_out8(0x01f7, 0x20);	// read with retry

	while (_io_in8(0x01f7) == 8);

	size_t total_words = num_sectors * 256;	//  256 bytes per sector

	for (int i = 0; i < total_words; i++) {
		buffw[i] = _io_in16(0x01f0);
	}

	return total_words << 1;
}
