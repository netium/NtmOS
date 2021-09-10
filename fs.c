#include "kern_basic.h"
#include "fs.h"
#include "harddisk.h"
#include "kernel_functions.h"
#include "kstring.h"

#define N_MAX_PARTITIONS (4)

filesystem_t g_root_filesystem = {0};

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

typedef struct {
	char filename[8];
	char extension[3];
	uint8_t attribute;
	uint8_t reserved_for_nt;
	uint8_t creation_msec_stamp;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t last_access_date;
	uint16_t reserved_for_fat32;
	uint16_t last_write_time;
	uint16_t last_write_date;
	uint16_t start_cluster;
	uint32_t file_size;
} fat16_directory_entry_t;

int parse_mbr(hdd_device_t *hdd, partition_t* p_partition);

int build_root_fs(hdd_device_t *hdd);

int build_root_fs_from_fat16(filesystem_t * p_root_fs);		

uint32_t get_location_of_n_fat_copy(filesystem_t *p_fs, uint8_t n);

uint32_t get_number_of_fat_entries(filesystem_t *p_fs);

uint32_t get_fat_sector_for_n_cluster_entry(filesystem_t *p_fs, uint32_t n);

uint32_t get_first_sector_of_cluster_n(filesystem_t *p_fs, uint32_t n);

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

	fat16_bootsector_t *fat16bs = &p_root_fs->fat16_bootsector;
	fat16_region_info_t *fat16region = &p_root_fs->fat16_region_info;

	fat16region->reserved_region_start = 0;
	fat16region->reserved_region_size = fat16bs->reserved_sectors;
	fat16region->fat_region_start = fat16region->reserved_region_start + fat16region->reserved_region_size;
	fat16region->fat_region_size = fat16bs->number_of_fat_copies * fat16bs->sectors_per_fat;
	fat16region->root_directory_region_start = fat16region->fat_region_start + fat16region->fat_region_size;
	fat16region->root_directory_region_size = ((fat16bs->number_of_root_entries * 32) / fat16bs->byte_per_sector);
	fat16region->data_region_start = fat16region->root_directory_region_start + fat16region->root_directory_region_size;
	fat16region->data_region_size = fat16bs->large_number_of_sectors - (fat16region->reserved_region_size + fat16region->fat_region_size + fat16region->root_directory_region_size);

	char str[100];
	k_sprintf(str, "boot sector signature is :%x, %x", p_root_fs->fat16_bootsector.media_descriptor, p_root_fs->fat16_bootsector.reserved_sectors);
	k_printf(str);

	return 0;	
}

inline uint32_t get_location_of_n_fat_copy(filesystem_t *p_fs, uint8_t n) {

	p_fs->fat16_region_info.reserved_region_start + (n * p_fs->fat16_bootsector.sectors_per_fat);
}

inline uint32_t get_number_of_fat_entries(filesystem_t *p_fs) {

	return p_fs->fat16_region_info.data_region_size / p_fs->fat16_bootsector.sectors_per_cluster;
}

inline uint32_t get_fat_sector_for_n_cluster_entry(filesystem_t *p_fs, uint32_t n) {

	return get_location_of_n_fat_copy(p_fs, 0) + ((n * 2) / p_fs->fat16_bootsector.byte_per_sector);
}

inline uint32_t get_first_sector_of_cluster_n(filesystem_t *p_fs, uint32_t n) {
	return p_fs->fat16_region_info.data_region_start + ((n - 2) * p_fs->fat16_bootsector.sectors_per_cluster);
}

int32_t get_file_size(char* filename) {
	if (NULL == filename)
		return -1;

	if (k_strlen(filename) == 0)
		return -1;

	
	
}