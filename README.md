# NtmOS

## Introduction

A very mini OS kernel, including the MBR boot sector, BL2 (ntmio.sys) and the kernel (kernel.sys).

## Build on Ubuntu

To build the NtmOS on Ubuntu 20.10, the following tools/libraries/packages are needed:

- gcc
- mtools
- make
- nasm >= 2.15 (The Ubuntu 20.10 built in one is 2.14 which will lack some of the support, so need to download the latest version from nasm official website)
- gcc-multilib/g++-multilib (To build 32bit by gcc on 64bit Linux)

## How to Build

It's simple, go to the source code directory, and run the following command:

````bash
# make clean || make
````

If nothing went wrong, then you will get a 'ntmos.img' file, this is the floppy image generated, which is built as FAT12 with MBR, BL2 and kernel in it.

Just use any virtual machine software to create a virtual machine and mount this file as floppy disk and boot from it, then it will work.

