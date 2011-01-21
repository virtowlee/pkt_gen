# Makefile for IPGen
# Author: Leo Liang

#DEBUG  :=      -g -DDEBUG
#OPTIMIZE:=      -O2
CFLAGS  :=      $(OPTIMIZE) $(DEBUG) -Wall
LDLIBS      = -lpthread

CC	:=	gcc
AR	:=	ar rc
AR2	:=	ranlib

OBJS	:=	ipgen.o main.o log.o timer.o
targets	:=	pkg_gen	

all:	$(targets)

$(targets): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

%.o:  %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS)
	rm -f $(targets)
	rm -f core*

