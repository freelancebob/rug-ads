#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

void* safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    fprintf(stderr, "Memory allocation failed.\n");
    abort();
  }
  return ptr;
}

void* safe_realloc(void *ptr, size_t size) {
  void *newPtr = realloc(ptr, size);
  if (!newPtr) {
    free(ptr);
    fprintf(stderr, "Memory reallocation failed.\n");
    abort();
  }
  return newPtr;
}
