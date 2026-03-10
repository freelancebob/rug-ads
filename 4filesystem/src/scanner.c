#include "scanner.h"

#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads one line from stdin into *buf, growing *capacity as needed.
// The string in the buffer ends with "\n\0".
// Returns false when EOF is reached with no input, true otherwise.
void read_line(char **buf, int *capacity) {
    int len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len == *capacity) {
            *capacity *= 2;
            *buf = safe_realloc(*buf, *capacity + 2);
        }
        (*buf)[len++] = c;
    }
    if (c == EOF && len != 0) {
        fprintf(stderr, "Error reading input line\n");
        exit(EXIT_FAILURE);
    }
    (*buf)[len++] = '\n';
    (*buf)[len] = '\0';
}

// This function tokenizes the next token on the input string.
// The token is assumed to start at *input_buf, and ends at the first occurence of
//      one of the characters in the breakset (:= *p).
// This function is destructive. The character at *p is replaced by '\0',
//      which makes *input_buf to *p a null-terminated string.
// The pointer to this string, along with its length, is returned as a Token.
// The pointer *input_buf is advanced to the first character past *p,
//      which sets it up for tokenizing the next token in the input string.
// If no character in the breakset is found, the returned Token holds a NULL pointer.
Token tokenize(char **input_buf, char const *breakset) {
    char *p = strpbrk(*input_buf, breakset);
    if (!p)
        return (Token) { .str = NULL };

    *p = '\0';
    Token t = { .str = *input_buf, .len = p - *input_buf };
    *input_buf = p + 1;
    return t;
}
