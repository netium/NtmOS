#ifndef _FS_H_
#define _FS_H_

#include "harddisk.h"

// For harddisk MBR structure, refer to: https://en.wikipedia.org/wiki/Master_boot_record#PT
// For FAT16 partition, refer to: http://www.maverick-os.dk/FileSystemFormats/FAT16_FileSystem.html#:~:text=The%20%EE%80%80FAT16%20file%20system%EE%80%81%20uses%20a%2016-bit%20number,The%20first%20is%20used%20on%20volumes%20with%20

#pragma pack(push, 1)
typedef struct {
	uint8_t code[3];
	uint8_t os_name[8];
	uint16_t byte_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t number_of_fat_copies;
	uint16_t number_of_root_entries;
	uint16_t small_number_of_sectors;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t number_of_heads;
	uint32_t hidden_sectors;
	uint32_t large_number_of_sectors;
	uint8_t drive_number;
	uint8_t reserved;
	uint8_t extended_boot_signature;
	uint32_t volume_serial_number;
	char volume_label[11];
	char file_system_type[8];
	uint8_t bootstrap_code[448];
	uint8_t boot_sector_signature[2];
} fat16_bootsector_t;
#pragma pack(pop)

typedef struct {
	// Unit is sector
	uint32_t reserved_region_start;
	uint32_t fat_region_start;
	uint32_t root_directory_region_start;
	uint32_t data_region_start;
	uint32_t reserved_region_size;
	uint32_t fat_region_size;
	uint32_t root_directory_region_size;
	uint32_t data_region_size;
} fat16_region_info_t;

typedef struct {
	hdd_device_t *hdd;
	uint8_t type;
	uint32_t start_lba;
	uint32_t num_sectors;
} partition_t;

typedef struct {
	partition_t device;
	fat16_bootsector_t fat16_bootsector;
	fat16_region_info_t fat16_region_info;
} filesystem_t;

extern filesystem_t g_root_filesystem;

int init_root_filesystem();

int partition_read(partition_t* p_partition, uint8_t * buff, size_t sector, size_t num_sectors);
int partition_write(partition_t* p_partition, uint8_t *buff, size_t sector, size_t num_sectors);

int32_t get_file_size(const char* filename);
int32_t get_file_start_cluster(const char* filename);
size_t	read_file(const char* filename, char *buf, size_t buf_size);

#endif