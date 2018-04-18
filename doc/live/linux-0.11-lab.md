
# ** Learning Linux 0.11 in Linux 0.11 Lab **

- Author: Wu Zhangjin / Falcon
- Time  : 2017/09/23 14:00 ~ 15:00
- Doc   :
    + <http://bochs.sf.net/doc/docbook/user/internal-debugger.html>
    + <https://github.com/tinyclub/linux-0.11-lab>
    + <http://tinylab.org/linux-0.11-lab>
    + <http://showdesk.io/50bc346f53a19b4d1f813b428b0b7b49>
    + <http://showterm.io/ffb67385a07fd3fcec182>
    + <http://showterm.io/4b628301d2d45936a7f8a>
    + <http://tinylab.org/lad-book>
    + <http://tinylab.cloud:6080>
    + <http://oldlinux.org>
    + README.md

## Introduction

1. Linux 0.11 Lab
    - Docker, one image everywhere
    - Runnable on Qemu and Bochs
    - Prebuilt ramfs, floppy and harddisk images
    - Work with latest gcc
    - Function calling tree support
    - Online available: noVNC(webVNC) and Gateone(webssh)

2. Linux 0.11
    - <http://oldlinux.org>
    - <http://oldlinux.org/Linux.old/>
    - <http://www.oldlinux.org/download/clk011c-3.0.pdf>

## Quick Start

1. Compiling
    - `make`
    - `make V=1`

2. Booting
    - `make boot`

3. Booting with rootfs from ram, floppy or harddisk
    - `make boot`
    - `make boot-fd`
    - `make boot-hd`

4. Booting on Qemu or Bochs
    - `make boot VM=qemu`
    - `make boot VM=bochs`
    - `make switch`: switch between Qemu and Bochs

## Debugging

### Debugging with gdb

Host:

    $ make debug

GDB:

    $ b main
    $ b fork
    $ s


If want to debug `src/boot/bootsect.sym` or `src/boot/setup.sym`, please pass a
DST variable:

    $ make debug DST=src/boot/bootsect.sym

### Debugging with bochs internal debugger

    $ cat src/kernel.map | grep time_init
    0000651e t time_init

    $ make boot-hd BOCHS=bochs-debugger
    Next at t=0
    (0) [0x0000fffffff0] f000:fff0 (unk. ctxt): jmp far f000:e05b         ; ea5be000f0
    <bochs:1> help
    h|help - show list of debugger commands
    h|help command - show short command description
    -*- Debugger control -*-
        help, q|quit|exit, set, instrument, show, trace, trace-reg,
        trace-mem, u|disasm, ldsym, slist
    -*- Execution control -*-
        c|cont|continue, s|step, p|n|next, modebp, vmexitbp
    -*- Breakpoint management -*-
        vb|vbreak, lb|lbreak, pb|pbreak|b|break, sb, sba, blist,
        bpe, bpd, d|del|delete, watch, unwatch
    -*- CPU and memory contents -*-
        x, xp, setpmem, writemem, crc, info,
        r|reg|regs|registers, fp|fpu, mmx, sse, sreg, dreg, creg,
        page, set, ptime, print-stack, ?|calc
    -*- Working with bochs param tree -*-
        show "param", restore
    <bochs:3> vb 0x0000:0x7c00
    <bochs:4> lb 0x0000
    <bochs:5> ldsym global "src/kernel.map"
    <bochs:6> c
    (0) Breakpoint 73809960, in 0000:7c00 (0x00007c00)
    Next at t=17995700
    (0) [0x000000007c00] 0000:7c00 (T do_device_not_available+1b): jmp far 07c0:0005         ; ea0500c007
    <bochs:7> c
    (0) Breakpoint 2, 0x00000000 in ?? ()
    Next at t=35555890
    (0) [0x000000000000] 0008:00000000 (T pg_dir+0): mov eax, 0x00000010       ; b810000000
    <bochs:8> n
    Next at t=35555891
    (0) [0x000000000005] 0008:00000005 (T pg_dir+5): mov ds, ax                ; 8ed8
    <bochs:9> b 0x651e
    <bochs:10> c
    (0) Breakpoint 3, 0x0000651e in ?? ()
    Next at t=52399578
    (0) [0x00000000651e] 0008:0000651e (t time_init+0): sub esp, 0x0000004c       ; 83ec4c

## Transfering files between Host and Guest

### From Host to Guest

Host:

    $ cd examples/
    $ <adding new files here>
    $ make boot-hd

Guest:

    $ cd /usr/root/
    $ ls examples/

### From Guest to Host

Guest:

    $ cd /usr/root/
    $ <adding new files here>

Host:

    $ make hd-mount
    $ sudo ls rootfs/_hda/usr/root/
    $ make hd-umount

## Adding a new syscall

Host:

    $ cd src
    $ patch -p1 < ../examples/syscall/syscall.patch
    $ cd ../

    $ make
    $ make boot-hd

Guest:

    $ cd examples/syscall
    $ make
    Hello, Linux 0.11

## Generating function calling tree

    $ make cg f=main
    $ ls callgraph/main.__init_main_c.svg

## Booting both of Image and Rootfs from harddisk

### Compile Linux 0.11 in Guest

Host:

    $ make boot-hd

Guest:

    $ cd examples/linux-0.11/
    $ make
    $ cat /etc/config | grep ^boot
    boot /usr/root/examples/linux-0.11/Image

### Boot everything from harddisk

Host:

    $ make switch VM=qemu
    $ make hd-boot

Note: This target only support qemu currently.
