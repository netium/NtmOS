#ifndef _HARDDISK_H_
#define _HARDDISK_H_

void init_harddisks();
int ata_lba_read(uint8_t * buff, size_t sector_lba, size_t num_sectors); 

#endif