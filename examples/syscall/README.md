
# How to add a new system call

## Host Side

1. Enter into the lab root directory

        $ cd /path/to/linux-0.11-lab/

2. Apply the syscall changes

        $ patch -p1 < examples/syscall/syscall.patch

3. Compile and boot the new kernel and filesystem

        $ make start-hd

    The above command will compile kernel, install examples/ to hardisk image and boot them.

## Emulator Side

6. Compile, link and run

        $ cd examples/syscall/
        $ make
        as -o syscall.o syscall.s
        ld -o syscall syscall.o
        ./syscall
        Hello, Linux 0.11 
