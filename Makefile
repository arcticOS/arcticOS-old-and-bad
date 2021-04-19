CC=/usr/bin/avr-gcc
MEGA=2560
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega$(MEGA) -Iinclude -I/usr/avr/sys-root/include
SOURCE=$(shell find source/ -name *.c)

all:
	$(CC) $(CFLAGS) $(SOURCE) -o build/arcticOS.o

clean :
	rm -f build/*
