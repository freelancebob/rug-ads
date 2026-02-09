/* based on stackTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include "LibStack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Stack newStack(int s) {
  Stack st;
  st.array = malloc(s * sizeof(*st.array));
  assert(st.array != NULL);
  st.top = 0;
  st.size = s;
  return st;
}

void doubleStackSize(Stack *stp) {
  int newSize = 2 * stp->size;
  stp->array = realloc(stp->array, newSize * sizeof(*stp->array));
  assert(stp->array != NULL);
  stp->size = newSize;
}

int isEmptyStack(Stack st) {
  return (st.top == 0);
}

void stackEmptyError(void) {
  printf("stack empty\n");
  abort();
}

void push(EditOperation value, Stack *stp) {
  if (stp->top == stp->size) {
    doubleStackSize(stp);
  }
  stp->array[stp->top] = value;
  stp->top++;
}

EditOperation pop(Stack *stp) {
  if (isEmptyStack(*stp)) {
    stackEmptyError();
  }
  stp->top--;
  return stp->array[stp->top];
}

void freeStack(Stack st) {
  free(st.array);
}
