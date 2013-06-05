CC=gcc
#CFLAGS=-Wall
LDFLAGS=

all: rtty mod-rtty
	
rtty: rtty.c
	$(CC) $(LDFLAGS) -o $@ $<

mod-rtty: mod-rtty.c
	$(CC) -lm -lportaudio $(LDFLAGS) -o $@ $<

clean:
	rm -rf rtty mod-rtty
