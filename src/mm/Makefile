
include ../Makefile.head

LDFLAGS += -r
CFLAGS	+= -I../include
CPP	+= -I../include

.c.o:
	$(Q)$(CC) $(CFLAGS) -c -o $*.o $<
.s.o:
	$(Q)$(AS) $(AFLAGS) -o $*.o $<
.c.s:
	$(Q)$(CC) $(CFLAGS) -S -o $*.s $<

OBJS	= memory.o page.o

all: mm.o

mm.o: $(OBJS)
	$(Q)$(LD) $(LDFLAGS) -o mm.o $(OBJS)

clean:
	$(Q)rm -f core *.o *.a tmp_make
	$(Q)for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	$(Q)sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	$(Q)(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	$(Q)cp tmp_make Makefile

### Dependencies:
memory.o: memory.c ../include/signal.h ../include/sys/types.h \
  ../include/asm/system.h ../include/linux/sched.h \
  ../include/linux/head.h ../include/linux/fs.h ../include/linux/mm.h \
  ../include/linux/kernel.h
