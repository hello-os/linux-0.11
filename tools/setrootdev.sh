#!/bin/bash
# setrootdev.sh -- set the root device in Image file
# author: falcon <wuzhangjin@gmail.com>
# update: 2008-12-25

image=$1
root_dev=$2
ram_img=$3

# by default, using the integrated floppy including boot & root image
if [ -z "$root_dev" ]; then
  DEFAULT_MAJOR_ROOT=0
  DEFAULT_MINOR_ROOT=0
else
  DEFAULT_MAJOR_ROOT=${root_dev:0:2}
  DEFAULT_MINOR_ROOT=${root_dev:2:3}
fi

# Set "device" for the root image file
echo -ne "\x$DEFAULT_MINOR_ROOT\x$DEFAULT_MAJOR_ROOT" | dd ibs=1 obs=1 count=2 seek=508 of=$image conv=notrunc >/dev/null 2>&1

# Write Ramdisk RootFS, block size defined in src/include/linux/fs.h, see BLOCK_SIZE
if [ -n "$ram_img" -a -f "$ram_img" ]; then
  dd if=$ram_img seek=$RAMDISK_START bs=1024 of=$image conv=notrunc >/dev/null 2>&1
fi
