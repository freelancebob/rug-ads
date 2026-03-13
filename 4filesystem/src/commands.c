#include "commands.h"
#include "filesystem.h"
#include "scanner.h"
#include "memory.h"

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
    return -1;
    // exit(EXIT_FAILURE);
}

// Remove the final element of a path token (usually a new file name) and returns it. Destructively edits the path token to exclude the final element. Returned string MUST BE FREED!
char *splitFileNameFromPath(Token path) {
    // remove the final part of the path (the new name) and store it
    char *filename = safe_malloc(sizeof(char) * FILE_NAME_SIZE);
    for (int i = path.len - 1; i >= 0; i --) {
        if (path.str[i] == '/') {
            strncpy(filename, &path.str[i + 1], path.len - i);

            path.str[i] = '\0';
            path.len = i;

            break;
        } else if (i == 0) {
            strcpy(filename, path.str);

            path.str[0] = '\0';
            path.len = 0;

            break;
        }
    }

    return filename;
}

void cmd_exit(FileNode *root) {
    deleteFile(root);
}

void cmd_ls(char **input_buf, FileNode *root) {
    // Read DIRPATH, or NULL when no dir is given
    Token dir = tokenize(input_buf, "\n");

    if (dir.str == NULL){
        for (int i = 0; i < root->fileCount; i++){
            FileNode *temp = root->children[i];
            printf("%s\n", temp->name);
        }
    } else {
        FileNode *found = getFileFromPath(root, dir.str);
        if (found != NULL) {
            for (int i = 0; i < found->fileCount; i++){
                FileNode *tempFound = found->children[i];
                printf("%s\n", tempFound->name); 
            }
        }
    }
}

void cmd_cat(char **input_buf, FileNode *root) {
    Token filepath = tokenize(input_buf, "\n");

    FileNode *found = getFileFromPath(root, filepath.str);
    if (found != NULL) {
        if (strcmp(found->contents, "") != 0) {
            printf("%s\n", found->contents);
        }
    } else {
        printf("BAD\n");
    }
}

void cmd_touch(char **input_buf, FileNode *root) {
    Token filepath = tokenize(input_buf, " \n");

    do {
        //
        char *filename = splitFileNameFromPath(filepath);
        FileNode *parent = getFileFromPath(root, filepath.str);

        if (parent != NULL) {
            if (parent->type == Directory) {
                newRegularFile(filename, parent);
            } else {
                // printf("%s is not a directory.\n", filepath.str);
            }
        } else {
            // printf("%s is an invalid path.\n", filepath.str);
        }
        
        free(filename);

        // Try to read next FILEPATH
        filepath = tokenize(input_buf, " \n");
    } while (filepath.str);
}

void cmd_echo(char **input_buf, FileNode *root) {
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
    FileNode *found = getFileFromPath(root, filepath.str);

    // create a new file if it doesnt exist yet
    if (found == NULL) {
        char *filename = splitFileNameFromPath(filepath);
        FileNode *parent = getFileFromPath(root, filepath.str);

        found = newRegularFile(filename, parent);
        free(filename);
    }

    if (found->type != Regular) {
        printf("Cannot write to directory file.\n");
        return;
    }

    //Selects the correct function based on the amount of >'s
    if (overwriteOrAppend.len == 1) {
        // ">"
        writeFileContents(found, string.str);
        
    } else {
        // ">>"
        appendFileContents(found, string.str);
    }
}

void cmd_mkdir(char **input_buf, FileNode *root) {
    // Advance past "-p "
    *input_buf += 3;

    // The loop repeatedly tokenizes DIRPATH until the end of the line.
    // Read first DIRPATH
    Token dir = tokenize(input_buf, " \n");
    do {
        FileNode *directory = root;
        if (dir.str[0] == '/') {
            while (directory->parent != NULL) {
                directory = directory->parent;
            }
        }

        // iterate through all filenames in the given path
        char *token = strtok(dir.str, "/");

        while (token != NULL) {
            if (strcmp(token, ".") == 0) {
                token = strtok(NULL, "/");
                continue;
            }
            
            if (strcmp(token, "..") == 0) {
                if (directory->parent != NULL) {
                    directory = directory->parent;
                }
                token = strtok(NULL, "/");
                continue;
            }

            // First check if a file already exists here
            bool found = false;
            for (int i = 0; i < directory->fileCount; i ++) {
                if (strcmp(directory->children[i]->name, token) == 0) {
                    directory = directory->children[i];
                    found = true;
                    break;
                }
            }

            // Otherwise create a new directory file
            if (found == false) {
                FileNode *newDir = newDirectoryFile(token, directory);
                directory = newDir;
            }

            token = strtok(NULL, "/");
        }

        // Read optional subsequent DIRPATHs
        dir = tokenize(input_buf, " \n");
    } while (dir.str);
}

void cmd_mv(char **input_buf, FileNode *root) {
    Token path1 = tokenize(input_buf, " ");
    Token path2 = tokenize(input_buf, " \n");

    // Paths are the same, so skip
    if (strcmp(path1.str, path2.str) == 0) {
        return;
    }

    // Process PATH_1
    FileNode *fileToMove = getFileFromPath(root, path1.str);
    if (fileToMove == NULL) {
        printf("%s is not a valid directory.\n", path1.str);
        return;
    }

    // Process PATH_2

    char *newFilename = splitFileNameFromPath(path2);
    FileNode *newParent = getFileFromPath(root, path2.str);

    // check if a file with this name exists in the new path and delete it
    for (int i = 0; i < newParent->fileCount; i ++) {
        if (strcmp(newParent->children[i]->name, newFilename) == 0) {
            deleteFile(newParent->children[i]);
            break;
        }
    }

    strcpy(fileToMove->name, newFilename);
    moveFileToParent(fileToMove, newParent);
    free(newFilename);
}

void cmd_cp(char **input_buf, FileNode *root) {
    // If we see a '-', advance past "-r ".
    if (**input_buf == '-') {
        *input_buf += 3;
    }

    // Read PATH_1
    Token path1 = tokenize(input_buf, " ");
    FileNode *fileToCopy = getFileFromPath(root, path1.str);
    if (fileToCopy == NULL) {
        printf("%s is not a valid directory.\n", path1.str);
        return;
    }

    // Read PATH_2
    Token path2 = tokenize(input_buf, " \n");
    
    char *newFilename = splitFileNameFromPath(path2);
    FileNode *newParent = NULL;

    if (path2.len > 0) {
        newParent = getFileFromPath(root, path2.str);
    } else {
        newParent = root;
        while (newParent->parent != NULL) {
            newParent = newParent->parent;
        }   
    }

    if (newParent == NULL) {
        printf("%s is not a valid directory.\n", path2.str);
        return;
    }

    // check if a file with this name exists in the new path and delete it
    for (int i = 0; i < newParent->fileCount; i ++) {
        if (strcmp(newParent->children[i]->name, newFilename) == 0) {
            deleteFile(newParent->children[i]);
            break;
        }
    }

    copyFile(fileToCopy, newParent, newFilename);
    free(newFilename);
}

void cmd_rm(char **input_buf, FileNode *root) {
    // If we see a '-', advance past "-r ".
    if (**input_buf == '-') {
        *input_buf += 3;
    }

    // The loop repeatedly tokenizes PATH until the end of the line.
    Token path = tokenize(input_buf, " \n");
    do {
        FileNode *found = getFileFromPath(root, path.str);

        if (found != NULL) {
            deleteFile(found);
        }

        path = tokenize(input_buf, " \n");
    } while (path.str);
}

void cmd_cd(char **input_buf, FileNode **workingDirectory) {
    // Read DIRPATH
    Token dir = tokenize(input_buf, " \n");

    // TODO: Replace with an implementation
    if (dir.len > 0) {
        FileNode *newDirectory = getFileFromPath(*workingDirectory, dir.str);
        if (newDirectory == NULL) {
            printf("Not a valid directory.");
        } else {
            *workingDirectory = newDirectory;
        }
    } else {
        // Repeatedly find parent until the root directory is reached
        FileNode *thisDirectory = *workingDirectory;
        while (thisDirectory->parent != NULL) {
            thisDirectory = thisDirectory->parent;
        }
        *workingDirectory = thisDirectory;
    }
}
    
void cmd_find_recursive(FileNode *root, char *prefix) {
    // Adds this file name to the prefix str (eg. from "/home", changes to "/home/etc")
    char *newPrefix = safe_malloc(strlen(prefix) + strlen(root->name) + 2);
    strcpy(newPrefix, prefix);
    strcat(newPrefix, "/");
    strcat(newPrefix, root->name);

    printf("%s\n", newPrefix); 
    
    // this will already be in alphabetical order
    for (int i = 0; i < root->fileCount; i ++) {
        cmd_find_recursive(root->children[i], newPrefix);
    }
    
    free(newPrefix);
}

void cmd_find(FileNode *root) {
    printf(".\n");
    // The first step will have a unique name (".") so use this function as a wrapper for the real recursive step
    for (int i = 0; i < root->fileCount; i ++) {
        cmd_find_recursive(root->children[i], ".");
    }
}

void cmd_ln(char **input_buf, FileNode *ds) {
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
