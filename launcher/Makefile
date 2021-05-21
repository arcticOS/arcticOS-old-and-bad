CC=/usr/bin/gcc
CFLAGS=-O2 -Wall -Iinclude
SOURCE=$(shell find source/ -name *.c)

all:
	$(CC) $(CFLAGS) $(SOURCE) -o build/launcher.bin

emulator:
	$(CC) $(CFLAGS) -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -DEMULATOR $(SOURCE) -o build/launcher.bin

clean :
	rm -f build/*
