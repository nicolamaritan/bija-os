#include "paging.h"
#include "memory/heap/kernel_heap.h"

extern void paging_enable_paging();
extern void paging_load_directory(page_directory* directory);

static page_directory* current_directory = NULL;

struct paging_4gb_chunk* paging_init_chunk(uint8_t flags)
{
    page_directory* directory = kernel_zalloc(sizeof(uint32_t) * PAGING_TABLES_PER_DIRECTORY);
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
    struct paging_4gb_chunk* chunk = kernel_zalloc(sizeof(struct paging_4gb_chunk));
    chunk->directory = directory;
    return chunk;
}

void paging_switch_current_directory(page_directory* directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}