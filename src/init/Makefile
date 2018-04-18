include ../Makefile.head

LDFLAGS += -r
CFLAGS	+= -I../include
CPP	+= -I../include

ifneq ($(RAMDISK_SIZE),)
  CFLAGS += -DRAMDISK_SIZE=$(RAMDISK_SIZE)
endif

.c.o:
	$(Q)$(CC) $(CFLAGS) -c -o $*.o $<

clean:
	$(Q)rm -f core *.o *.a tmp_make
	$(Q)for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	$(Q)sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	$(Q)(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	$(Q)cp tmp_make Makefile

### Dependencies:
main.o: main.c ../include/unistd.h ../include/sys/stat.h \
 ../include/sys/types.h ../include/sys/times.h ../include/sys/utsname.h \
 ../include/utime.h ../include/time.h ../include/linux/tty.h \
 ../include/termios.h ../include/linux/sched.h ../include/linux/head.h \
 ../include/linux/fs.h ../include/linux/mm.h ../include/signal.h \
 ../include/asm/system.h ../include/asm/io.h ../include/stddef.h \
 ../include/stdarg.h ../include/fcntl.h
