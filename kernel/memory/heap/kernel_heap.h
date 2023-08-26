#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

void kernel_heap_init();
void* kernel_malloc(size_t size);

#endif