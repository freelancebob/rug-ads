#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token {
    char *str;
    int len;
} Token;

void read_line(char **buf, int *capacity);
Token tokenize(char **input_buf, char const *breakset);

#endif
