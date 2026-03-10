#include "memory.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_INITIAL_SIZE 512  // The size in number of chars.
#define NUMBER_OF_COMMANDS 12


/* TODO: Rename this data structure and all references to it,
 * adapt its contents to your needs, and move it into its own header and
 * source file. Add the name of the new source file (i.e. *.c) to the
 * Makefile as well, next to the other *.c files.
 */
typedef struct DataStructure {
    int a;
    int b;
    int c;
} DataStructure;


typedef struct Token {
    char *str;
    int len;
} Token;

/* This function tokenizes the next token on the input string.
 * The token is assumed to start at *input_buf, and ends at the first occurence
 * of one of the characters in the breakset (:= *p).
 * This function is **destructive**. The character at *p is replaced by '\0',
 * which makes *input_buf to *p a null-terminated string. The pointer to this string, along with
 * its length, is returned as a Token. The pointer *input_buf is advanced to the first
 * character past *p, which sets it up for tokenizing the next token in the input string.
 * If no character in the breakset is found, the returned Token holds a NULL pointer.
 */
Token tokenize(char **input_buf, char const *breakset) {
    char *p = strpbrk(*input_buf, breakset);
    if (!p)
        return (Token) { .str = NULL };

    *p = '\0';
    Token t =  { .str = *input_buf, p - *input_buf };
    *input_buf = p + 1;
    return t;
}

void cmd_exit(DataStructure *ds) {
    // TODO: Adapt this destruction to suit your data structure.
    free(ds);

    // TODO: Remove
    puts("cmd_exit.");
}

void cmd_ls(char **input_buf, DataStructure *ds) {
    // Read DIRECTORY, or NULL when no dir is given
    Token dir = tokenize(input_buf, "\n");

    // TODO: Replace with an implementation
    if (dir.str) {
        printf("cmd_ls. dir: %s (len = %d)\n", dir.str, dir.len);
    } else {
        puts("cmd_ls. No dir given, use the root dir (or current working dir if you do the corresponding bonus assignment)");
    }
}

void cmd_cat(char **input_buf, DataStructure *ds) {
    Token file_name = tokenize(input_buf, "\n"); // Read FILENAME

    // TODO: Replace with an implementation
    printf("cmd_cat. file_name: %s (len = %d)\n", file_name.str, file_name.len);
}

void cmd_touch(char **input_buf, DataStructure *ds) {
    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes FILENAME until the end of the line.
    printf("cmd_touch. File_names: ");
    Token file_name = tokenize(input_buf, " \n"); // Read first FILENAME
    do {
        printf("%s (len = %d) | ", file_name.str, file_name.len);

        file_name = tokenize(input_buf, " \n"); // Try to read next FILENAME
    } while (file_name.str); // While a next FILENAME exists
    putchar('\n');
}

void cmd_echo(char **input_buf, DataStructure *ds) {
    ++*input_buf; // Advance past the first '"'
    Token string = tokenize(input_buf, "\""); // Read STRING until '"'
    ++*input_buf; // Advance past the ' '
    Token overwriteOrAppend = tokenize(input_buf, " "); // Read ">" or ">>"
    Token file_name = tokenize(input_buf, " \n"); // Read FILENAME

    // TODO: Replace with an implementation
    if (overwriteOrAppend.len == 1) { // ">"
        printf("cmd_echo. str: %s (len = %d) > file_name: %s (len = %d)\n",
               string.str, string.len, file_name.str, file_name.len);
    } else { // ">>"
        printf("cmd_echo. str: %s (len = %d) >> file_name: %s (len = %d)\n",
               string.str, string.len, file_name.str, file_name.len);
    }
}

void cmd_mkdir(char **input_buf, DataStructure *ds) {
    *input_buf += 3; // Advance past "-p "

    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes DIRECTORY until the end of the line.
    printf("cmd_mkdir. Dirs: ");
    Token dir = tokenize(input_buf, " \n"); // Read first DIRECTORY
    do {
        printf("%s (len = %d) | ", dir.str, dir.len);

        dir = tokenize(input_buf, " \n"); // Read optional subsequent DIRECTORYs
    } while (dir.str);
    putchar('\n');
}

void cmd_mv(char **input_buf, DataStructure *ds) {
    Token path1 = tokenize(input_buf, " "); // Read PATH_1
    Token path2 = tokenize(input_buf, " \n"); // Read PATH_2

    // TODO: Replace with an implementation
    printf("cmd_mv. path1: %s (len = %d) | path2: %s (len = %d)\n",
           path1.str, path1.len, path2.str, path2.len);
}

void cmd_cp(char **input_buf, DataStructure *ds) {
    if (**input_buf == '-') // If we see a '-',
        *input_buf += 3; // advance past "-r ".

    Token path1 = tokenize(input_buf, " "); // Read PATH_1
    Token path2 = tokenize(input_buf, " \n"); // Read PATH_2

    // TODO: Replace with an implementation
    printf("cmd_cp. path1: %s (len = %d) | path2: %s (len = %d)\n",
           path1.str, path1.len, path2.str, path2.len);
}

void cmd_rm(char **input_buf, DataStructure *ds) {
    if (**input_buf == '-') // If we see a '-',
        *input_buf += 3; // advance past "-r ".

    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes PATH until the end of the line.
    printf("cmd_rm. Paths: ");
    Token path = tokenize(input_buf, " \n"); // Read first PATH
    do {
        printf("%s (len = %d) | ", path.str, path.len);

        path = tokenize(input_buf, " \n"); // Read optional subsequent PATH
    } while (path.str);
    putchar('\n');
}

void cmd_cd(char **input_buf, DataStructure *ds) {
    Token dir = tokenize(input_buf, " \n"); // Read DIRECTORY

    // TODO: Replace with an implementation
    if (dir.str) {
        printf("cmd_cd. dir: %s (len = %d)\n", dir.str, dir.len);
    } else {
        puts("cmd_cd. No dir given. Set the working directory to the root directory");
    }
}

void cmd_find(DataStructure *ds) {
    // TODO: Replace with an implementation
    puts("cmd_find");
}

void cmd_ln(char **input_buf, DataStructure *ds) {
    bool make_symlink = false;
    if (**input_buf == '-') { // If we see a '-',
        *input_buf += 3; // advance past "-s ".
        make_symlink = true;
    }

    Token file_name1 = tokenize(input_buf, " "); // Read FILENAME_1
    Token file_name2 = tokenize(input_buf, " \n"); // Read FILENAME_2

    // TODO: Replace with an implementation
    if (make_symlink) {
        printf("cmd_ln (-s). path1: %s (len = %d) | path2: %s (len = %d)\n",
               file_name1.str, file_name1.len, file_name2.str, file_name2.len);
    } else { // make a hard link
        printf("cmd_ln. path1: %s (len = %d) | path2: %s (len = %d)\n",
               file_name1.str, file_name1.len, file_name2.str, file_name2.len);
    }
}

/* Reads one line from the input into a provided buffer and enlarges its capacity
 * if necessary. The string in the buffer ends with "\n\0".
 */
void read_line(char **buf, int *capacity) {
    int len = 0;
    int c;
    while ((c = getchar()) && c != '\n' && c != EOF) {
        if (len == *capacity) {
            *capacity *= 2;
            *buf = safe_realloc(*buf, *capacity + 2);
        }
        (*buf)[len++] = c;
    }
    (*buf)[len++] = '\n';
    (*buf)[len] = '\0';
}
// The index returned here corresponds with the ones in the switch case in main().
int find_cmd_idx(char const *cmd) {
    // The roman numerals correspond with the numbering in the assignment.
    char const *cmd_strs[NUMBER_OF_COMMANDS] = {
        "exit",  // I
        "ls",    // II
        "cat",   // III
        "touch", // IV
        "echo",  // V
        "mkdir", // VI
        "mv",    // VII
        "cp",    // VIII
        "rm",    // IX
        "cd",    // X
        "find",  // XI
        "ln"     // XII
    };

    // Try to match the input command string to a command and return the index.
    for (int i = 0; i < NUMBER_OF_COMMANDS; ++i)
        if (strcmp(cmd, cmd_strs[i]) == 0)
            return i;

    fprintf(stderr, "Command not found");
    exit(EXIT_FAILURE);
}

int main(void) {
    // The input buffer and its capacity. Reused for every line of input.
    // Allocate memory for the additional "\n\0" at the end.
    int input_buf_capacity = INPUT_BUFFER_INITIAL_SIZE;
    char *input_buf = safe_malloc((input_buf_capacity + 2) * sizeof(char));

    // Initialize the data structure
    // TODO: Adapt this initialization to your needs
    DataStructure *data_structure = safe_malloc(sizeof(DataStructure));
    data_structure->a = 13;
    data_structure->b = 37;
    data_structure->c = 42;

    bool isRunning = true;
    while (isRunning) {
        read_line(&input_buf, &input_buf_capacity);

        // Command parsing
        // The cmd_* functions modify the input_buf pointer parameter,
        // so we pass it a copy so that the original can be free'd later.
        char *buf_ptr_cpy = input_buf;
        Token cmd_token = tokenize(&buf_ptr_cpy, " \n");

        // Find and call the function corresponding to the command.
        int cmd_idx = find_cmd_idx(cmd_token.str);
        switch (cmd_idx) {
        case  0: cmd_exit(data_structure); isRunning = false; break;
        case  1: cmd_ls(&buf_ptr_cpy, data_structure); break;
        case  2: cmd_cat(&buf_ptr_cpy, data_structure); break;
        case  3: cmd_touch(&buf_ptr_cpy, data_structure); break;
        case  4: cmd_echo(&buf_ptr_cpy, data_structure); break;
        case  5: cmd_mkdir(&buf_ptr_cpy, data_structure); break;
        case  6: cmd_mv(&buf_ptr_cpy, data_structure); break;
        case  7: cmd_cp(&buf_ptr_cpy, data_structure); break;
        case  8: cmd_rm(&buf_ptr_cpy, data_structure); break;
        case  9: cmd_cd(&buf_ptr_cpy, data_structure); break;
        case 10: cmd_find(data_structure); break;
        case 11: cmd_ln(&buf_ptr_cpy, data_structure); break;
        }
    }

    // Clean up the input buffer.
    free(input_buf);
}
