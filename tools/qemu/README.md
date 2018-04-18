
qemu 0.10 for X86 on Linux.

Built from [Linux Lab](https://github.com/tinyclub/linux-lab)

    $ vim machines/pc/Makefile  // uncomment the following line
    QEMU = 0.10

    $ apt-get install -y pkg-config libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev libgtk-3-dev libsdl1.2-dev
    $ make MACH=pc
    $ make emulator
