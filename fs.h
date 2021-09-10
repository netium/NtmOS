#ifndef _FS_H_
#define _FS_H_

// For harddisk MBR structure, refer to: https://en.wikipedia.org/wiki/Master_boot_record#PT

typedef struct {
	hdd_device_t *hdd;
	uint8_t type;
	uint32_t start_lba;
	uint32_t num_sectors;
} partition_t;

typedef struct {
	partition_t device;
} filesystem_t;

filesystem_t g_root_filesystem;

int init_root_filesystem();

#endif