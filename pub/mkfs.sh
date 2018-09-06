#!/bin/sh 
./tools/mkfs.jffs2  -d ./rootfs_uclibc -l -e 0x40000 -o ./image_uclibc/rootfs_uclibc_256k.jffs2
./tools/mkyaffs2image610  ./rootfs_uclibc  ./image_uclibc/rootfs_uclibc_2k_4bit.yaffs2 1 2  
