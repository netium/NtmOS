#include "kern_basic.h"
#include "kernel_functions.h"
#include "harddisk.h"

// The whole implemenation is refer to: https://wiki.osdev.org/ATA_PIO_Mode

enum port_offset {
	DATA_REGISTER_OFFSET = 0,
	ERROR_REGISTER_OFFSET = 1,
	FEATURE_REGISTER_OFFSET = 1,
	SECTOR_COUNT_REGISTER_OFFSET = 2,
	LBA_LO_REGISTER_OFFSET = 3,
	SECTOR_NUMBER_REGISTER_OFFSET = 3,
	LBA_MID_REGISTER_OFFSET = 4,
	CYLINDER_LOW_REGISTER_OFFSET = 4,
	LBA_HI_REGISTER_OFFSET = 5,
	CYLINDER_HIGH_REGISTER_OFFSET = 5,
	DRIVE_HEAD_REGISTER_OFFSET = 6,
	STATUS_REGISTER_OFFSET = 7,
	COMMAND_REGISTER_OFFSET = 8
};

enum ata_pio_error_register_bit {
	AMNF = 1 << 0,
	TKZNF = 1 << 1,
	ABRT = 1 << 2,
	MCR = 1 << 3,
	IDNF = 1 << 4,
	MC = 1 << 5,
	UNC = 1 << 6,
	BBK = 1 << 7
};

enum ata_pio_status_register_bit {
	ERR = 1 << 0,	// Indicates an error occurred, Send a new command to clear it (or nuke it with a Software Reset)
	IDX = 1 << 1,	// Index, always set to zero
	CORR = 1 << 2,	// Corrected data. Always set to zero
	DRQ = 1 << 3,	// Set when the drive has PIO data to transfer, or is ready to accept PIO data.
	SRV = 1 << 4,	// Overlapped Mode Service Request.
	DF = 1 << 5,	// Drive Fault Error (does not set ERR).
	RDY = 1 << 6,	// Bit is clear when drive is spun down, or after an error. Set otherwise.
	BSY = 1 << 7,	// Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
};

#define N_MAX_HDD (4)
hdd_device_t s_hdd_devices[N_MAX_HDD] = {0};

int ata_detect();
int ata_detect_hdd(uint16_t port, int is_primary, uint16_t *identify_data_buf);

void init_harddisks() {
	ata_detect();

	k_printf("HDD detection complete!");

	hdd_device_t *p_hdd_device = NULL;
	for (int i = 0; i < N_MAX_HDD; i++) {
		if (s_hdd_devices[i].is_present) {
			p_hdd_device = &s_hdd_devices[i];
			break;
		}
	}

	if (p_hdd_device != NULL) {
		uint8_t buff[512];
		ata_pio_lba_read(p_hdd_device, buff, 0, 1);

		char str[256];
		for (int i = 0; i < 0x10; i++) {
			k_sprintf(str, "hdd[%x] read: [%x]", i, buff[i]);
			k_printf(str);
		}
	}	
}

int ata_detect() {
	uint16_t scan_ports[] = {0x1f0, 0x170};

	for (int i = 0; i < N_MAX_HDD; i++) {
		s_hdd_devices[i].is_present = 0;
	}

	char str[256];

	int n = 0;
	for (int i = 0; i < sizeof(scan_ports) / sizeof(scan_ports[0]); i++) {
		int ret = ata_detect_hdd(scan_ports[i], 1, s_hdd_devices[n].identify_words);
		if (ret == 0) {
			s_hdd_devices[n].io_port_base = scan_ports[i];	
			s_hdd_devices[n].is_present = 1;
			s_hdd_devices[n].is_primary = 1;
			n++;
			k_sprintf(str, "HDD found on port [%x]: Primary", scan_ports[i]);
			k_printf(str);
		}
		ret = ata_detect_hdd(scan_ports[i], 0, s_hdd_devices[n].identify_words);
		if (ret == 0) {
			s_hdd_devices[n].io_port_base = scan_ports[i];
			s_hdd_devices[n].is_present = 1;
			s_hdd_devices[n].is_primary= 0;
			n++;
			k_sprintf(str, "HDD found on port [%x]: Secondary", scan_ports[i]);
			k_sprintf(str);
		}
	}
}

int ata_detect_hdd(uint16_t port, int is_primary, uint16_t *identify_data_buf) {

	_io_out8(port + DRIVE_HEAD_REGISTER_OFFSET, is_primary ? 0xa0 : (0xa0 | 0x10));
	_io_out8(port + SECTOR_COUNT_REGISTER_OFFSET, 0x00);
	_io_out8(port + LBA_LO_REGISTER_OFFSET, 0x00);
	_io_out8(port + LBA_MID_REGISTER_OFFSET, 0x00);
	_io_out8(port + LBA_HI_REGISTER_OFFSET, 0x00);
	_io_out8(port + STATUS_REGISTER_OFFSET, 0xec);

	uint8_t status = _io_in8(port + STATUS_REGISTER_OFFSET);

	while (_io_in8(port + STATUS_REGISTER_OFFSET) & BSY == 1);

	if (status == 0) {
		return -1;
	}

	uint8_t lba_mid = _io_in8(port + LBA_MID_REGISTER_OFFSET);
	uint8_t lba_hi = _io_in8(port + LBA_HI_REGISTER_OFFSET);

	if (lba_mid != 0 && lba_hi != 0) {
		return -1;
	}

	while (_io_in8(port + STATUS_REGISTER_OFFSET) & DRQ == 0);

	for (int i = 0; i < 256; i++) {
		identify_data_buf[i] = _io_in16(port); 
	}
	return 0;
}

int ata_pio_lba_read(hdd_device_t* hdd, uint8_t * buff, size_t abs_lba_sector, size_t num_sectors) {

	if (NULL == hdd || !hdd->is_present)
		_panic();

	uint16_t * buffw = (uint16_t *)buff;

	uint8_t drive_head_value = (uint8_t)(((abs_lba_sector >> 24) & 0x0f) | 0xe0);

	// if (!hdd->is_primary)
	//	drive_head_value |= 0x10;

	_io_out8(hdd->io_port_base + DRIVE_HEAD_REGISTER_OFFSET, drive_head_value);	// port to send drive and bit 24-27 of LBA, also set bit 6 for LBA mode

	_io_out8(hdd->io_port_base + SECTOR_COUNT_REGISTER_OFFSET, (uint8_t)num_sectors);	// send number of sectors

	_io_out8(hdd->io_port_base + LBA_LO_REGISTER_OFFSET, (uint8_t)abs_lba_sector);

	_io_out8(hdd->io_port_base + LBA_MID_REGISTER_OFFSET, (uint8_t)(abs_lba_sector >> 8));

	_io_out8(hdd->io_port_base + LBA_HI_REGISTER_OFFSET, (uint8_t)(abs_lba_sector >> 16));

	_io_out8(hdd->io_port_base + STATUS_REGISTER_OFFSET, 0x20);	// read with retry

	while (_io_in8(hdd->io_port_base + STATUS_REGISTER_OFFSET) & BSY != 0); 

	while (_io_in8(hdd->io_port_base + STATUS_REGISTER_OFFSET) & DRQ == 0);

	size_t total_words = num_sectors * 256;	//  256 bytes per sector

	for (int i = 0; i < total_words; i++) {
		buffw[i] = _io_in16(hdd->io_port_base);
	}

	return total_words << 1;
}
