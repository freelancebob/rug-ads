/* based on stackTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#ifndef LIBSTACK_H
#define LIBSTACK_H

#include "EditOperation.h"

typedef struct Stack {
  EditOperation *array;
  int top;
  int size;
} Stack;

Stack newStack(int s);
void doubleStackSize(Stack *stp);
int isEmptyStack(Stack st);
void push(EditOperation value, Stack *stp);
EditOperation pop(Stack *stp);
void freeStack(Stack st);

#endif
