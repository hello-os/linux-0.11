#
# Makefile for the FREAX-kernel character device drivers.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

include ../../Makefile.head

CFLAGS	+= -I../../include
CPP	+= -I../../include

.c.s:
	$(Q)$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	$(Q)$(AS) $(AFLAGS) -o $*.o $<
.c.o:
	$(Q)$(CC) $(CFLAGS) -c -o $*.o $<

OBJS  = tty_io.o console.o keyboard.o serial.o rs_io.o \
	tty_ioctl.o

chr_drv.a: $(OBJS)
	$(Q)$(AR) rcs chr_drv.a $(OBJS)
	$(Q)sync

keyboard.s: kb.S ../../include/linux/config.h
	$(Q)$(CPP) kb.S -o keyboard.s

clean:
	$(Q)rm -f core *.o *.a tmp_make keyboard.s
	$(Q)for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	$(Q)sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	$(Q)(for i in *.c;do echo -n `echo $$i | sed 's,\.c,\.s,'`" "; \
		$(CPP) -M $$i;done) >> tmp_make
	$(Q)cp tmp_make Makefile

### Dependencies:
console.s console.o: console.c ../../include/linux/sched.h \
  ../../include/linux/head.h ../../include/linux/fs.h \
  ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/signal.h ../../include/linux/tty.h \
  ../../include/termios.h ../../include/asm/io.h \
  ../../include/asm/system.h
serial.s serial.o: serial.c ../../include/linux/tty.h ../../include/termios.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/sys/types.h \
  ../../include/linux/mm.h ../../include/signal.h \
  ../../include/asm/system.h ../../include/asm/io.h
tty_io.s tty_io.o: tty_io.c ../../include/ctype.h ../../include/errno.h \
  ../../include/signal.h ../../include/sys/types.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/linux/mm.h \
  ../../include/linux/tty.h ../../include/termios.h \
  ../../include/asm/segment.h ../../include/asm/system.h
tty_ioctl.s tty_ioctl.o: tty_ioctl.c ../../include/errno.h ../../include/termios.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/sys/types.h \
  ../../include/linux/mm.h ../../include/signal.h \
  ../../include/linux/kernel.h ../../include/linux/tty.h \
  ../../include/asm/io.h ../../include/asm/segment.h \
  ../../include/asm/system.h
