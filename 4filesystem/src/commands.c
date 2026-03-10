#include "commands.h"

#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_COMMANDS 12

// Returns the index (0-based) for the given command string.
// Exits with an error on an unknown command.
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

    for (int i = 0; i < NUMBER_OF_COMMANDS; ++i) {
        if (strcmp(cmd, cmd_strs[i]) == 0) {
            return i;
        }
    }

    fprintf(stderr, "Command not found\n");
    exit(EXIT_FAILURE);
}

void cmd_exit(DataStructure *ds) {
    // TODO: Adapt this destruction to suit your data structure.
    free(ds);

    // TODO: Remove
    puts("cmd_exit.");
}

void cmd_ls(char **input_buf, DataStructure *ds) {
    // Read DIRPATH, or NULL when no dir is given
    Token dir = tokenize(input_buf, "\n");

    // TODO: Replace with an implementation
    if (dir.str) {
        printf("cmd_ls. dir: %s (len = %d)\n", dir.str, dir.len);
    } else {
        puts("cmd_ls. No dir given, use the root dir (or the current working dir if you do the corresponding bonus assignment)");
    }
}

void cmd_cat(char **input_buf, DataStructure *ds) {
    // Read FILEPATH
    Token filepath = tokenize(input_buf, "\n");

    // TODO: Replace with an implementation
    printf("cmd_cat. file_name: %s (len = %d)\n", filepath.str, filepath.len);
}

void cmd_touch(char **input_buf, DataStructure *ds) {
    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes FILEPATH until the end of the line.
    printf("cmd_touch. Filepaths: ");
    // Read first FILEPATH
    Token filepath = tokenize(input_buf, " \n");
    // While a next FILEPATH exists
    do {
        printf("%s (len = %d) | ", filepath.str, filepath.len);
        // Try to read next FILEPATH
        filepath = tokenize(input_buf, " \n");
    } while (filepath.str);
    putchar('\n');
}

void cmd_echo(char **input_buf, DataStructure *ds) {
    // Advance past the first '"'
    ++*input_buf;
    // Read STRING until '"'
    Token string = tokenize(input_buf, "\"");
    // Advance past the ' '
    ++*input_buf;
    // Read ">" or ">>"
    Token overwriteOrAppend = tokenize(input_buf, " ");
    // Read FILEPATH
    Token filepath = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    if (overwriteOrAppend.len == 1) {
        // ">"
        printf("cmd_echo. str: %s (len = %d) > filepath: %s (len = %d)\n", string.str, string.len, filepath.str, filepath.len);
    } else {
        // ">>"
        printf("cmd_echo. str: %s (len = %d) >> filepath: %s (len = %d)\n", string.str, string.len, filepath.str, filepath.len);
    }
}

void cmd_mkdir(char **input_buf, DataStructure *ds) {
    // Advance past "-p "
    *input_buf += 3;

    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes DIRPATH until the end of the line.
    printf("cmd_mkdir. Dirs: ");
    // Read first DIRPATH
    Token dir = tokenize(input_buf, " \n");
    do {
        printf("%s (len = %d) | ", dir.str, dir.len);
        // Read optional subsequent DIRPATHs
        dir = tokenize(input_buf, " \n");
    } while (dir.str);
    putchar('\n');
}

void cmd_mv(char **input_buf, DataStructure *ds) {
    // Read PATH_1
    Token path1 = tokenize(input_buf, " ");
    // Read PATH_2
    Token path2 = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    printf("cmd_mv. path1: %s (len = %d) | path2: %s (len = %d)\n", path1.str, path1.len, path2.str, path2.len);
}

void cmd_cp(char **input_buf, DataStructure *ds) {
    // If we see a '-', advance past "-r ".
    if (**input_buf == '-') {
        *input_buf += 3;
    }

    // Read PATH_1
    Token path1 = tokenize(input_buf, " ");
    // Read PATH_2
    Token path2 = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    printf("cmd_cp. path1: %s (len = %d) | path2: %s (len = %d)\n", path1.str, path1.len, path2.str, path2.len);
}

void cmd_rm(char **input_buf, DataStructure *ds) {
    // If we see a '-', advance past "-r ".
    if (**input_buf == '-') {
        *input_buf += 3;
    }

    // TODO: Replace with an implementation.
    // The loop repeatedly tokenizes PATH until the end of the line.
    printf("cmd_rm. Paths: ");
    // Read first PATH
    Token path = tokenize(input_buf, " \n");
    do {
        printf("%s (len = %d) | ", path.str, path.len);
        // Read optional subsequent PATH
        path = tokenize(input_buf, " \n");
    } while (path.str);
    putchar('\n');
}

void cmd_cd(char **input_buf, DataStructure *ds) {
    // Read DIRPATH
    Token dir = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    if (dir.str) {
        printf("cmd_cd. dir: %s (len = %d)\n", dir.str, dir.len);
    } else {
        puts("cmd_cd. No dir given. Set the working directory to the root directory");
    }
}

void cmd_find(DataStructure *ds) {
    // TODO: Replace with an implementation
    puts("cmd_find.");
}

void cmd_ln(char **input_buf, DataStructure *ds) {
    bool make_symlink = false;
    // If we see a '-', advance past "-s ".
    if (**input_buf == '-') {
        *input_buf += 3;
        make_symlink = true;
    }

    // Read FILEPATH_1
    Token filepath1 = tokenize(input_buf, " ");
    // Read FILEPATH_2
    Token filepath2 = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    if (make_symlink) {
        printf("cmd_ln -s. filepath1: %s (len = %d) | filepath2: %s (len = %d)\n", filepath1.str, filepath1.len, filepath2.str, filepath2.len);
    } else {
        // make a hard link
        printf("cmd_ln. filepath1: %s (len = %d) | filepath2: %s (len = %d)\n", filepath1.str, filepath1.len, filepath2.str, filepath2.len);
    }
}
