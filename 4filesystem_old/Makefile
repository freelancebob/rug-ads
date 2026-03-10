CC = gcc

CFLAGS = -g -std=c99 -O2 -pedantic -Wall

fs: main.c memory.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: clean

clean:
	rm -f fs
