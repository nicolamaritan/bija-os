#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

/**
 * @brief Initializes the kernel heap.
 *
 * This function sets up the kernel heap, including allocating memory for the heap
 * table and initializing the heap structure.
 */
void kernel_heap_init();

/**
 * @brief Allocates a block of memory from the kernel heap.
 *
 * @param size Size of the memory block to allocate.
 * @return A pointer to the allocated memory block if successful, or NULL if there's not enough memory.
 */
void* kernel_malloc(size_t size);

/**
 * @brief Frees a previously allocated memory block in the kernel heap.
 *
 * @param ptr Pointer to the memory block to free.
 */
void kernel_free(void* ptr);

void* kernel_zalloc(size_t size);
#endif