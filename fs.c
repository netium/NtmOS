#include "kern_basic.h"
#include "fs.h"
#include "harddisk.h"
#include "kernel_functions.h"

int build_root_fs(hdd_device_t *hdd);

int init_root_filesystem() {
	for (int i = 0; i < N_MAX_HDD; i++) {
		if (g_hdd_devices[i].is_present) {
			build_root_fs(&g_hdd_devices[i]);
			return 0;
		}
	}

	return -1;
}

int parse_mbr(hdd_device_t *hdd);
int build_root_fs(hdd_device_t *hdd) {
	parse_mbr(hdd);
}

// MBR starts here

int parse_mbr(hdd_device_t *hdd) {
	uint8_t mbr[512];
	ata_pio_lba_read(hdd, mbr, 0, 1);
	if (mbr[510] != 0x55 || mbr[511] != 0xAA) {
		k_printf("Not an MBR disk");
		_panic();
	}
}