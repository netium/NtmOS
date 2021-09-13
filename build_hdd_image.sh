#! /usr/bin/sh

# Refer to https://wiki.osdev.org/Loopback_Device

dd if=/dev/zero of=/var/ntmos/images/hdd.img bs=1M count=50

parted /var/ntmos/images/hdd.img mktable msdos

parted -s /var/ntmos/images/hdd.img mkpart primary fat16 1MiB 49MiB

losetup -o1048576 /dev/loop8 /var/ntmos/images/hdd.img


