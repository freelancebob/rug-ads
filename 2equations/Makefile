CC = gcc

CFLAGS = -O2 -std=c99 -pedantic -Wall -o -g -lm

run: mainRecog.c scanner.c recognizeEq.c
	$(CC) $(CFLAGS) $^ -o $@

scan: mainScan.c scanner.c
	$(CC) $(CFLAGS) $^ -o $@

recogExp: expRecog.c scanner.c recognizeExp.c
	$(CC) $(CFLAGS) $^ -o $@

eval: scanner.c recognizeExp.c evalExp.c mainEvalExp.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean debug-scan

clean:
	rm -f eval recog scan

debug-scan: scan
	cat example_part1_input.txt | valgrind ./scan
