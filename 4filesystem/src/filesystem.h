#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define INITIAL_CHILDREN_LIMIT 4
#define INITIAL_CONTENTS_LIMIT 16
#define FILE_NAME_SIZE 61

typedef enum FileType {
  Regular,
  Directory
} FileType;

typedef struct FileNode {
    char name[FILE_NAME_SIZE];
    FileType type;
    struct FileNode* parent;

    // Fields relevant for "Regular" files:
    char *contents;
    int contentsLimit;

    // Fields relevant for "Directory" files:
    struct FileNode **children;
    int fileCount;
    int fileLimit;
} FileNode;

FileNode *newRegularFile(char name[FILE_NAME_SIZE], FileNode *parent);
FileNode *newDirectoryFile(char name[FILE_NAME_SIZE], FileNode *parent);
FileNode *newRootDirectoryFile(void);
FileNode *getFileFromPath(FileNode *directory, char *path);
FileNode *copyFile(FileNode *file, FileNode *newParent, char newName[FILE_NAME_SIZE]);
void writeFileContents(FileNode *file, char *contents);
void appendFileContents(FileNode *file, char *string);
void moveFileToParent(FileNode *file, FileNode *parent);
void deleteFile(FileNode *file);

#endif // FILESYSTEM.H
