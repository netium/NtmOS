#ifndef _HARDDISK_H_
#define _HARDDISK_H_

typedef struct {
	uint16_t io_ports_base;
	uint16_t control_port_base;
	int is_primary;
	uint8_t is_present;
	uint8_t identify_words[256];
} hdd_device_t;

void init_harddisks();
int ata_pio_lba_read(uint8_t * buff, size_t sector_lba, size_t num_sectors); 

#endif