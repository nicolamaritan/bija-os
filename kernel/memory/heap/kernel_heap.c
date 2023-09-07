#include <kernel/memory/heap/heap.h>
#include <kernel/memory/heap/kernel_heap.h>
#include <kernel/config.h>
#include <kernel/tty.h>
#include <kernel/kernel.h>
#include <kernel/memory/memory.h>

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kernel_heap_init()
{
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(HEAP_TABLE_ADDRESS);
    kernel_heap_table.entries_number = HEAP_SIZE / HEAP_BLOCK_SIZE;

    void* end = (void*)(HEAP_ADDRESS + HEAP_SIZE);
    int result = heap_create(&kernel_heap, (void*)(HEAP_ADDRESS), end, &kernel_heap_table);
    if (result < 0)
    {
        print("Failed to create heap\n");
    }

}

void* kernel_malloc(size_t size)
{
    return heap_malloc(&kernel_heap, size);
}

void kernel_free(void* ptr)
{
    return heap_free(&kernel_heap, ptr);
}

void* kernel_zalloc(size_t size)
{
    void* ptr = kernel_malloc(size);
    if (!ptr)
        return NULL;
    memset(ptr, 0, size);
    return ptr;
    
}