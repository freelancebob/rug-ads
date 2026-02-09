#ifndef EDIT_OPERATION_H
#define EDIT_OPERATION_H

#include <stdio.h>

typedef enum EditOperationType {
  INSERT,
  DELETE
} EditOperationType;

typedef struct EditOperation {
  EditOperationType type;
  char character;
  int position;
} EditOperation;

int inline editOperationEquals(EditOperation op1, EditOperation op2) {
  return op1.type == op2.type && op1.character == op2.character && op1.position == op2.position;
}

void inline printEditOperation(EditOperation op) {
  switch (op.type) {
  case INSERT:
    printf("Insert %c at position %d\n", op.character, op.position);
    break;
  case DELETE:
    printf("Delete at position %d\n", op.position);
    break;
  }
}

#endif
