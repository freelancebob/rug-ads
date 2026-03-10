CC = gcc

CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Werror=implicit-function-declaration

SRC = src/main.c src/memory.c src/scanner.c src/filesystem.c src/commands.c

filesystem: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test: filesystem
	./filesystem < input.txt

test-valgrind: filesystem
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./filesystem < input.txt

.PHONY: clean

clean:
	rm -f filesystem
