#ifndef COMMANDS_H
#define COMMANDS_H

#include "filesystem.h"

int find_cmd_idx(char const *cmd);
void cmd_exit(FileNode *ds);
void cmd_ls(char **input_buf, FileNode *ds);
void cmd_cat(char **input_buf, FileNode *ds);
void cmd_touch(char **input_buf, FileNode *ds);
void cmd_echo(char **input_buf, FileNode *ds);
void cmd_mkdir(char **input_buf, FileNode *ds);
void cmd_mv(char **input_buf, FileNode *ds);
void cmd_cp(char **input_buf, FileNode *ds);
void cmd_rm(char **input_buf, FileNode *ds);
void cmd_cd(char **input_buf, FileNode **ds);
void cmd_find(FileNode *ds);
void cmd_ln(char **input_buf, FileNode *ds);

#endif
