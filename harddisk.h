#ifndef _HARDDISK_H_
#define _HARDDISK_H_

#define N_MAX_HDD (4)

typedef struct {
	uint16_t io_port_base;
	uint16_t control_port_base;
	int is_primary;
	uint8_t is_present;
	uint8_t is_lba28_support;
	uint8_t is_lba48_support;
	uint32_t n_sector_lba28;
	uint64_t n_sector_lba48;
	uint8_t identify_words[256];
} hdd_device_t;

extern hdd_device_t g_hdd_devices[];

void init_harddisks();
int ata_pio_lba_read(hdd_device_t* hdd, uint8_t *buff, size_t sector_lba, size_t num_sectors); 
int ata_pio_lba_write(hdd_device_t* hdd, uint8_t *buff, size_t sector_lba, size_t num_sectors);

#endif