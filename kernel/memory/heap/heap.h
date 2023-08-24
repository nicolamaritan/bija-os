#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0X01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0X00
#define HEAP_BLOCK_HAS_NEXT 0x80
#define HEAP_BLOCK_IS_FREE 0x70

typedef uint8_t HEAP_BLOCK_TABLE_ENTRY;

struct heap_table
{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t size;
};

struct heap
{
    struct heap_table* table;
    void* saddr;
};

#endif