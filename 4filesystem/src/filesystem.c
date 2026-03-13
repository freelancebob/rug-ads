#include "filesystem.h"
#include "memory.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define EMPTY_STRING ""

// [INTERNAL] Doubles the size limit of the children
void doubleChildrenLimit(FileNode *file) {
    file->fileLimit = file->fileLimit * 2;
    file->children = safe_realloc(file->children, file->fileLimit * sizeof(*file->children));
}

// [INTERNAL] Doubles the size limit of the contents array
void doubleContentsLimit(FileNode *file) {
    file->contentsLimit = file->contentsLimit * 2;
    file->contents = safe_realloc(file->contents, file->contentsLimit * sizeof(char));
}

// [INTERNAL] Removes the given file from its parent
void removeFileFromParent(FileNode *file) {
    if (file->parent != NULL) {
        bool foundFile = false;
        for (int i = 0; i < file->parent->fileCount; i ++) {
            // once the file was found, flip "foundFile" to true
            // so now, shift all files after this one to the left (overwriting this file)
            if (strcmp(file->parent->children[i]->name, file->name) == 0) {
                foundFile = true;
            } else if (foundFile) {
                file->parent->children[i - 1] = file->parent->children[i];
            }
        }
        file->parent->fileCount -= 1;

        file->parent = NULL;
    }
}

// Removes the file from its current parent (if it has one) and adds the file as a child to a new parent
void moveFileToParent(FileNode *file, FileNode *parent) {
    removeFileFromParent(file);

    // if the children limit was reached, double the array size
    if (parent->fileCount >= parent->fileLimit) {
        doubleChildrenLimit(parent);
    }

    //Places the new file into the correct alphabetical order
    // It does this by shifting all files lexographically greater than this file to the right,
    // then places the new file into the hole created after shifting those files
    int holeIndex = parent->fileCount;
    for (int i = parent->fileCount - 1; i >= 0; i --) {
        if (strcmp(parent->children[i]->name, file->name) > 0) {
            parent->children[i + 1] = parent->children[i];
            holeIndex = i;
        } else {
            break;
        }
    }

    parent->children[holeIndex] = file;
    parent->fileCount += 1;
    file->parent = parent;
}

// [INTERNAL] Creates a new untyped file with the given name, without a parent
FileNode *newFileInternal(char name[FILE_NAME_SIZE]) {
    FileNode *newFileNode = safe_malloc(sizeof(FileNode));
    
    strcpy(newFileNode->name, name);

    // initialize the children array
    newFileNode->children = safe_malloc(sizeof(FileNode*) * INITIAL_CHILDREN_LIMIT);
    newFileNode->fileCount = 0;
    newFileNode->fileLimit = INITIAL_CHILDREN_LIMIT;

    // initialize the contents string
    newFileNode->contents = safe_malloc(sizeof(char) * INITIAL_CONTENTS_LIMIT);
    newFileNode->contentsLimit = INITIAL_CONTENTS_LIMIT;

    newFileNode->parent = NULL;

    return newFileNode;
}

// [INTERNAL] Creates a new untyped file with the given name, and adds it as a child to the given parent
FileNode *newFileWithParentInternal(char name[FILE_NAME_SIZE], FileNode *parent) {
    FileNode *newFileNode = newFileInternal(name);
    moveFileToParent(newFileNode, parent);    
    return newFileNode;
}

// Creates a new *Regular* file with the given name, and adds it as a child to the given parent
FileNode *newRegularFile(char name[FILE_NAME_SIZE], FileNode* parent) {
    FileNode *newFileNode = newFileWithParentInternal(name, parent);
    newFileNode->type = Regular;
    return newFileNode;
}

// Creates a new *Directory* file with the given name, and adds it as a child to the given parent
FileNode *newDirectoryFile(char name[FILE_NAME_SIZE], FileNode* parent) {
    FileNode *newFileNode = newFileWithParentInternal(name, parent);
    newFileNode->type = Directory;
    return newFileNode;
}

// Recursively copies a file and its contents, and returns the copy
FileNode *copyFile(FileNode *file, FileNode *newParent, char newName[FILE_NAME_SIZE]) {
    if (file->type == Directory) {
        // case: file is a directory file, so create a new directory and recursively copy its children into it
        FileNode *newDirFile = newDirectoryFile(newName, newParent);

        for (int i = 0; i < file->fileCount; i ++) {
            copyFile(file->children[i], newDirFile, file->children[i]->name);
        }

        return newDirFile;
    } else {
        // case: file is a regular file, so just create a new one and copy the contents to it
        FileNode *newRegFile = newRegularFile(newName, newParent);
        writeFileContents(newRegFile, file->contents);
        return newRegFile;
    }
}

// Overwrite the contents of a regular file
void writeFileContents(FileNode *file, char *contents) {
    size_t length = strlen(contents) + 1;
    while (length > file->contentsLimit) {
        doubleContentsLimit(file);
    } 
    strcpy(file->contents, contents);
}

// Append the new string to the given files contents
void appendFileContents(FileNode *file, char *string) {
    size_t length = strlen(file->contents) + strlen(string) + 1;
    while (length > file->contentsLimit) {
        doubleContentsLimit(file);
    }
    strcat(file->contents, string); 
}

// [INTERNAL] Deletes the contents of a file, but does not remove itself from its parent
void deleteFileInternal(FileNode *file) {
    for (int i = 0; i < file->fileCount; i ++) {
        deleteFileInternal(file->children[i]);
    }

    free(file->children);
    free(file->contents);
    free(file);
}

// Deletes a file, removing itself from it's parent and recursively deleting all it's children
void deleteFile(FileNode *file) {
    removeFileFromParent(file);
    deleteFileInternal(file);
}

FileNode *getFileFromPath(FileNode *directory, char *path) {
    // set directory to the root if the first character of path is "/"
    if (path[0] == '/') {
        while (directory->parent != NULL) {
            directory = directory->parent;
        }
    }

    char *copy = safe_malloc(strlen(path) + 1);
    strcpy(copy, path); 

    char *token = strtok(copy, "/");
    
    while (token != NULL) {
        bool found = false;

        //Stays in the same directory and goes to the next token
        if (strcmp(token, ".") == 0) {
            token = strtok(NULL, "/");
            continue;
        }

        //The directory becomes their parent and goes to the next token
        if (strcmp(token, "..") == 0) { 
            if (directory->parent != NULL) {
                directory = directory->parent;
            }

            token = strtok(NULL, "/");
            continue;
        }

        // Must be a directory to have children
        if (directory->type != Directory) {
            free(copy);
            return NULL;
        }

        // Loop through all the children and check if a file with the given name exists
        for (int j = 0; j < directory->fileCount; j ++) {
            if (strcmp(directory->children[j]->name, token) == 0) {
                found = true;
                directory = directory->children[j];
                break;
            }
        } 

        // no file by this name was found in one of the subdirectories
        if (found == false) {
            free(copy);
            return NULL;
        }

        token = strtok(NULL, "/");
    }
    
    free(copy);
    return directory;
}

// Returns a root directory file (a file with no parent, named "/")
FileNode *newRootDirectoryFile(void) {
    FileNode *rootDirectory = newFileInternal("/");
    rootDirectory->type = Directory;
    return rootDirectory;
}


