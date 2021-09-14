#! /usr/bin/sh

# Refer to https://wiki.osdev.org/Loopback_Device

if [ -e /var/ntmos/images/hdd.img ]; then
	rm -fr /var/ntmos/images/hdd.img
fi

dd if=/dev/zero of=/var/ntmos/images/hdd.img bs=1M count=50

parted -s /var/ntmos/images/hdd.img mktable msdos

parted -s /var/ntmos/images/hdd.img mkpart primary fat16 1MiB 49MiB

# Before this the docker need to restart otherwise it will say hdd.img is busy
# Need to think about how to fix this

loop_device=$(losetup -o1048576 --find --show /var/ntmos/images/hdd.img)

mkdosfs -F 16 -I $loop_device 

mount $loop_device /mnt

if [ -e /var/ntmos/userspace_app/simpleapp/simpleapp.bin ]; then
	cp /var/ntmos/userspace_app/simpleapp/simpleapp.bin /mnt/init
fi

umount /mnt

losetup -d $loop_device


