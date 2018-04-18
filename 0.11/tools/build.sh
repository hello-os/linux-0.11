#!/bin/bash
#
# build.sh -- a shell version of build.c for the new bootsect.s & setup.s
# author: falcon <wuzhangjin@gmail.com>
# update: 2008-10-10

bootsect=$1
setup=$2
kernel=$3
image=$4

# Write bootsect (512 bytes, 1 sector)
[ ! -f "$bootsect" ] && echo "Error: No bootsect binary file there" && exit -1
dd if=$bootsect bs=512 count=1 of=$image >/dev/null 2>&1

# Write setup (4 * 512bytes, 4 sectors) see SETUPLEN in boot/bootsect.s
[ ! -f "$setup" ] && echo "Error: No setup binary file there" && exit -1
dd if=$setup seek=1 bs=512 count=4 of=$image >/dev/null 2>&1

# Write kernel (< SYS_SIZE), see the hardcoded SYSSIZE in src/boot/bootsetc.s
[ ! -f "$kernel" ] && echo "Error: No kernel binary file there" && exit -1

# Verify RAMDISK_START, must reserve enough space for bootsect, setup and kernel itself.
# TODO: need to document why need 9 more blocks?

KERNEL_SIZE=$(stat -c%s $kernel | tr -d '\n')
_SYS_SIZE=$(( KERNEL_SIZE + 5*512))
_RAMDISK_START=$(( _SYS_SIZE / 1024 + 9))

if [ $_RAMDISK_START -gt $RAMDISK_START ]; then
    echo "Note: The kernel binary is too big, Please increase RAMDISK_START to (138, 410)"
    echo
    echo " e.g."
    echo "     $ make distclean"
    echo "     $ make boot RAMDISK_START=$_RAMDISK_START"
    echo
    exit -1
fi

dd if=$kernel seek=5 bs=512 of=$image >/dev/null 2>&1
