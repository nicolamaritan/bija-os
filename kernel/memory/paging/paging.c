#include "paging.h"
#include "memory/heap/kernel_heap.h"

/**
 * Load the page directory pointed by `directory`.
*/
extern void paging_load_directory(uint32_t* directory);
extern void paging_enable_paging();

static uint32_t* current_directory = NULL;

uint32_t* paging_make_directory(uint8_t flags)
{
    uint32_t* directory = kernel_zalloc(sizeof(uint32_t) * PAGING_TABLES_PER_DIRECTORY);
    size_t offset = 0;
    for(int i = 0; i < PAGING_TABLES_PER_DIRECTORY; i++)
    {
        uint32_t* table = kernel_zalloc(sizeof(uint32_t) * PAGING_PAGES_PER_TABLE);
        for (int j = 0; j < PAGING_PAGES_PER_TABLE; j++)
        {
            // Linear Mapping
            table[j] = ((j * PAGING_PAGE_SIZE) + offset) | flags;
        }
        offset += (PAGING_TABLES_PER_DIRECTORY * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t)table | flags | PAGING_IS_WRITABLE;
    }
    return directory;
}

void paging_switch_current_directory(uint32_t* directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}