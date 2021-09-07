#include "kern_basic.h"
#include "kernel_functions.h"
#include "harddisk.h"

int ata_detect();

void init_harddisks() {
	ata_detect();

	uint8_t buff[512];
	ata_lba_read(buff, 0, 1);	

	char str[256];
	for (int i = 0; i < 10; i++) {
		k_sprintf(str, "hdd[%x] read: [%x]", i, buff[i]);
		k_printf(str);
	}
}

int ata_detect() {
	_io_out8(0x1f6, 0xa0);
	_io_out8(0x1f2, 0x00);
	_io_out8(0x1f3, 0x00);
	_io_out8(0x1f4, 0x00);
	_io_out8(0x1f5, 0x00);
	_io_out8(0x1f7, 0xec);

	k_printf("Start to check BSY status");

	uint8_t status = _io_in8(0x1f7);

	while (_io_in8(0x1f7) & 0x80 == 1);

	if (status == 0) {
		k_printf("No harddisk on primary found!");
		return 0;
	}

	uint8_t lba_mid = _io_in8(0x1f4);
	uint8_t lba_hi = _io_in8(0x1f5);

	if (lba_mid != 0 && lba_hi != 0) {
		k_printf("No ATA harddisk on primary!");
		return 0;
	}

	k_printf("start to poll DRQ");

	while (_io_in8(0x1f7 & 0x8));

	uint16_t identify_data[256];

	for (int i = 0; i < 256; i++) {
		identify_data[i] = _io_in16(0x1f0); 
	}
}
// Implementation refer to https://wiki.osdev.org/ATA_read/write_sectors

int ata_lba_read(uint8_t * buff, size_t sector_lba, size_t num_sectors) {
	uint16_t * buffw = (uint16_t *)buff;

	_io_out8(0x01f6, (uint8_t)(((sector_lba >> 24) & 0x0f) | 0xe0));	// port to send drive and bit 24-27 of LBA, also set bit 6 for LBA mode

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
