#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

// Memory management helper functions
void *safeMalloc(size_t size);
void *safeRealloc(void *ptr, size_t size);

#endif // MEMORY_H
