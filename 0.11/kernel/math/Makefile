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

OBJS  = math_emulate.o

math.a: $(OBJS)
	$(Q)$(AR) rcs math.a $(OBJS)
	$(Q)sync

clean:
	$(Q)rm -f core *.o *.a tmp_make
	$(Q)for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	$(Q)sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	$(Q)(for i in *.c;do echo -n `echo $$i | sed 's,\.c,\.s,'`" "; \
		$(CPP) -M $$i;done) >> tmp_make
	$(Q)cp tmp_make Makefile

### Dependencies:
