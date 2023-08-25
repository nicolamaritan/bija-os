#include "heap.h"
#include "kernel_heap.h"
#include "config.h"
#include "tty/tty.h"
#include "kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kernel_heap_init()
{
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(HEAP_TABLE_ADDRESS);
    kernel_heap_table.size = HEAP_SIZE / HEAP_BLOCK_SIZE;

    void* end = (void*)(HEAP_ADDRESS + HEAP_SIZE);
    int result = heap_create(&kernel_heap, (void*)(HEAP_ADDRESS), end, &kernel_heap_table);
    if (result < 0)
    {
        print("Failed to create heap\n");
    }

}