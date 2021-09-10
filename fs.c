#include "kern_basic.h"
#include "fs.h"
#include "harddisk.h"
#include "kernel_functions.h"

#define N_MAX_PARTITIONS (4)
typedef struct st_partition_entry_t {
	uint8_t status;
	uint8_t first_abs_sector_chs_address[3];
	uint8_t partition_type;
	uint8_t last_abs_sector_chs_address[3];
	uint32_t first_abs_sector_lba_address;
	uint32_t number_of_sectors;
} mbr_partition_entry_t;

typedef struct modern_standard_mbr {
	uint8_t bootstrap_code_area_part1[218];
	uint8_t oem_loader_signature[6];
	uint8_t bootstrap_code_area_part2[216];
	uint8_t disk_signature[6];
	mbr_partition_entry_t partition_entries[N_MAX_PARTITIONS];
	uint8_t boot_signature[2];
} mbr_t;

int parse_mbr(hdd_device_t *hdd, partition_t* p_partition);

int build_root_fs(hdd_device_t *hdd);

int build_root_fs_from_fat16(filesystem_t * p_root_fs);		

int init_root_filesystem() {
	for (int i = 0; i < N_MAX_HDD; i++) {
		if (g_hdd_devices[i].is_present) {
			int ret = build_root_fs(&g_hdd_devices[i]);
			if (ret == 0) return 0;	
		}
	}

	return -1;
}

int build_root_fs(hdd_device_t *hdd) {
	if (0 != parse_mbr(hdd, &g_root_filesystem.device))
		return -1;

	// If it's FAT16, then build up root file system from it
	if (g_root_filesystem.device.type == 0x06) {
		build_root_fs_from_fat16(&g_root_filesystem);		
	}	

	return -1;
}

// MBR starts here

int parse_mbr(hdd_device_t *hdd, partition_t *p_partition) {
	if (NULL == hdd && NULL == p_partition)
		_panic();

	mbr_t mbr;
	ata_pio_lba_read(hdd, &mbr, 0, 1);
	if (mbr.boot_signature[0] != 0x55 || mbr.boot_signature[1] != 0xAA) {
		k_printf("Not an MBR disk");
		_panic();
	}

	mbr_partition_entry_t *pe = &mbr.partition_entries[0];

	// Find the first valid partition as root
	for (int i = 0; i < N_MAX_PARTITIONS; i++) {
		if (mbr.partition_entries[i].status < 0x01 || mbr.partition_entries[i].status > 0x7f) {
			pe = &mbr.partition_entries[i];
			break;
		}
	}

	if (NULL != pe) {
		p_partition->hdd = hdd;
		p_partition->type = pe->partition_type;
		p_partition->start_lba = pe->first_abs_sector_lba_address;
		p_partition->num_sectors = pe->number_of_sectors;		

		return 0;
	}

	return -1;
}

int partition_read(partition_t* p_partition, uint8_t * buff, size_t sector, size_t num_sectors) {
	if (NULL == p_partition)
		_panic();

	if (sector + num_sectors >= p_partition->num_sectors)
		_panic();

	return ata_pio_lba_read(p_partition->hdd, buff, p_partition->start_lba + sector, num_sectors);
}

int partition_write(partition_t* p_partition, uint8_t *buff, size_t sector, size_t num_sectors) {
	return -1;
}

int build_root_fs_from_fat16(filesystem_t * p_root_fs) {
	partition_read(&p_root_fs->device, (uint8_t *)&p_root_fs->fat16_bootsector, 0, 1);

	char str[100];
	k_sprintf(str, "boot sector signature is :%x, %x", p_root_fs->fat16_bootsector.sectors_per_fat, p_root_fs->fat16_bootsector.sectors_per_cluster);
	k_printf(str);

	return 0;	
}