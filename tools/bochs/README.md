
# Compiling bochs

## Introduction

In ubuntu 14.04, bochs 2.6.2 is tested.

    $ sudo apt-get install xorg-dev libgtk2.0-dev

Usage: <http://bochs.sf.net/doc/docbook/user/internal-debugger.html>

## Download bochs

* Homepage: <https://bochs.sourceforge.net>.
* Download: <https://sf.net/projects/bochs/files/bochs/>

## Decompress bochs

    $ tar zxf bochs-2.6.2.tar.gz
    $ cd bochs-2.6.2

## Compile it with gdbstub support

`tools/bochs/bochs` is compiled as following:

    $ ./configure --enable-all-optimizations --enable-disasm --enable-gdb-stub --with-x11 --with-term
    $ make

## Compile it with debugger support

`tools/bochs/bochs-debugger` is compiled as following:

    $ export LIBS=-lpthread CFLAGS=`pkg-config gtk+-2.0 --cflags --libs`
    $ ./configure --enable-all-optimizations --enable-disasm --enable-debugger --enable-x86-debugger
    $ make
