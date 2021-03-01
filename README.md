# NtmOS

## Introduction

A very mini operating system, which is based on example of 「30日でできる！ OS自作入門」

## Build on Ubuntu

To build the NtmOS on Ubuntu 20.10, the following tools/libraries/packages are needed:

- gcc
- mtools
- make
- nasm >= 2.15 (The Ubuntu 20.10 built in one is 2.14 which will lack some of the support, so need to download the latest version from nasm official website)
- gcc-multilib/g++-multilib (To build 32bit by gcc on 64bit Linux)