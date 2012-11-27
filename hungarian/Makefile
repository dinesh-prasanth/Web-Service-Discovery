CC = gcc
AR = ar

#CFLAGS = -g -Wall -I.
CFLAGS = -O3 -Wall -I.
LDFLAGS = -L. -lhungarian

HUNGLIB = libhungarian.a
VERSION = 0.2

all: $(HUNGLIB) test timetest

test: test.c $(HUNGLIB)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

timetest: timetest.c $(HUNGLIB)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lm

OBJS = hungarian.o makeprob.o

$(HUNGLIB): $(OBJS)
	$(AR) cr $@ $(OBJS)
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

dist: clean
	cd .. && rm -f libhungarian-$(VERSION).tar.gz && tar -cvzf libhungarian-$(VERSION).tar.gz hungarian --exclude libhungarian-$(VERSION).tar.gz --exclude CVS 
	mv ../libhungarian-$(VERSION).tar.gz .

clean:
	rm -f *.o *.a test
