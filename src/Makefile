# This is a basic Makefile for setting the general configuration
include Makefile.head

LDFLAGS	+= -Ttext 0 -e startup_32
CFLAGS	+= -Iinclude
CPP	+= -Iinclude

#
# ROOT_DEV specifies the default root-device when making the image.
# This can be either FLOPPY, /dev/xxxx or empty, in which case the
# default of hd1(0301) is used by 'build'.
#
#ROOT_DEV= 021d	# FLOPPY B
#ROOT_DEV= 0301	# hd1

ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
MATH	=kernel/math/math.a
LIBS	=lib/lib.a

.c.s:
	$(Q)$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	$(Q)$(AS) $(AFLAGS) -o $*.o $<
.c.o:
	$(Q)$(CC) $(CFLAGS) -c -o $*.o $<

all:	Image

Image: boot/bootsect boot/setup kernel.bin FORCE
	$(BUILD) boot/bootsect boot/setup kernel.bin Image
	$(Q)rm -f kernel.bin
	$(Q)sync

init/main.o: FORCE
	$(Q)(cd init; make $(S) main.o)

boot/head.o: boot/head.s FORCE
	$(Q)(cd boot; make $(S) head.o)

kernel.bin: kernel.sym
	$(Q)cp -f kernel.sym kernel.tmp
	$(Q)$(STRIP) kernel.tmp
	$(Q)$(OBJCOPY) -O binary -R .note -R .comment kernel.tmp kernel.bin
	$(Q)rm kernel.tmp

kernel.sym: boot/head.o init/main.o \
		$(ARCHIVES) $(DRIVERS) $(MATH) $(LIBS) FORCE
	$(Q)$(LD) $(LDFLAGS) boot/head.o init/main.o \
	$(ARCHIVES) \
	$(DRIVERS) \
	$(MATH) \
	$(LIBS) \
	-o kernel.sym
	$(Q)nm kernel.sym | grep -v '\(compiled\)\|\(\.o$$\)\|\( [aU] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)'| sort > kernel.map

kernel/math/math.a: FORCE
	$(Q)(cd kernel/math; make $(S))

kernel/blk_drv/blk_drv.a: FORCE
	$(Q)(cd kernel/blk_drv; make $(S))

kernel/chr_drv/chr_drv.a: FORCE
	$(Q)(cd kernel/chr_drv; make $(S))

kernel/kernel.o: FORCE
	$(Q)(cd kernel; make $(S))

mm/mm.o: FORCE
	$(Q)(cd mm; make $(S))

fs/fs.o: FORCE
	$(Q)(cd fs; make $(S))

lib/lib.a: FORCE
	$(Q)(cd lib; make $(S))

boot/setup: boot/setup.s FORCE
	$(Q)(cd boot; make $(S))

boot/bootsect: boot/bootsect.s kernel.bin FORCE
	$(Q)(cd boot; make $(S))

clean:
	$(Q)rm -f Image kernel.map tmp_make core boot/bootsect boot/setup
	$(Q)rm -f kernel.sym boot/*.o typescript* info bochsout.txt
	$(Q)for i in init mm fs kernel lib boot; do (cd $$i; make $(S) clean); done

distclean: clean
	$(Q)rm -f tag* cscope* linux-0.11.*

dep:
	$(Q)sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	$(Q)cp tmp_make Makefile
	$(Q)for i in init fs kernel mm; do (cd $$i; make $(S) dep); done

FORCE: ;
