#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void* safe_malloc(size_t size);
void* safe_realloc(void *ptr, size_t size);

#endif
