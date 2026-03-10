#include "commands.h"
#include "filesystem.h"
#include "memory.h"
#include "scanner.h"

#include <stdbool.h>
#include <stdlib.h>

#define INPUT_BUFFER_INITIAL_SIZE 512  // The size in number of chars.

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

    return EXIT_SUCCESS;
}
