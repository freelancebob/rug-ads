CC = gcc

CFLAGS = -g -std=c99 -O2 -pedantic -Wall

speller: dictionary.c speller.c memory.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: example check-example debug-example

example: speller
	./speller < exampleInput.txt

check-example: speller
	cat exampleInput.txt | ./speller | diff exampleOutput.txt -

debug-example: speller
	valgrind --leak-check=full ./speller < exampleInput.txt

time-largeExample: speller
	time ./speller < large_exampleInput.txt
