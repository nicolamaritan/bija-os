#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0X01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0X00
#define HEAP_BLOCK_HAS_NEXT 0x80
#define HEAP_BLOCK_IS_FIRST 0x70

typedef uint8_t HEAP_BLOCK_TABLE_ENTRY;

/**
 * @struct heap_table
 * @brief Represents a table for managing heap block entries.
 */
struct heap_table
{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t entries_number;
};

/**
 * @struct heap
 * @brief Represents a heap structure that manages memory allocation.
 */
struct heap
{
    struct heap_table* table;
    void* start_address;
};

/**
 * @brief Creates a heap structure.
 *
 * This function initializes a heap structure with the given parameters.
 *
 * @param heap Pointer to the heap structure to be created.
 * @param ptr Starting address of the memory space for the heap.
 * @param end Ending address of the memory space for the heap.
 * @param table Pointer to the heap table to be used with the heap.
 * @return 0 on success, or a negative error code on failure.
 */

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
/**
 * @brief Allocates memory from the heap.
 *
 * This function allocates memory of the specified size from the heap.
 *
 * @param heap Pointer to the heap structure.
 * @param size Size of the memory to allocate.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
void* heap_malloc(struct heap* heap, size_t size);

/**
 * @brief Frees allocated memory.
 *
 * This function deallocates a previously allocated memory block within the given heap.
 *
 * @param[in] heap A pointer to the heap from which to free the memory.
 * @param[in] ptr A pointer to the memory block to be freed.
 *
 * @note It is crucial to pass a valid `heap` pointer that was used for the allocation
 * of the `ptr` parameter.
 *
 * @warning Attempting to free memory that has already been freed or passing an invalid
 * `heap` pointer may result in undefined behavior.
 *
 */
void heap_free(struct heap* heap, void* ptr);

#endif