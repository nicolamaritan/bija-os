#include <kernel/memory/paging/paging.h>
#include <kernel/memory/heap/kernel_heap.h>
#include <kernel/status.h>

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

int paging_is_aligned(void* addr)
{
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
}

static int paging_get_table_index(void* virtual_address, size_t* table_index_out)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }
    *table_index_out = (uint32_t)virtual_address / (PAGING_PAGE_SIZE * PAGING_PAGES_PER_TABLE);
    return 0;
}

static int paging_get_page_index(void* virtual_address, size_t* page_index_out)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }
    size_t offset_from_table_start = (uint32_t)virtual_address % (PAGING_PAGE_SIZE * PAGING_PAGES_PER_TABLE);
    *page_index_out = offset_from_table_start / PAGING_PAGE_SIZE;
    return 0;
}

int paging_set_page_table_entry(uint32_t* directory, void* virtual_address, uint32_t value)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }
    size_t table_index;
    size_t page_index;
    int res;

    if ((res = paging_get_table_index(virtual_address, &table_index)) < 0)
    {
        return res;
    }
    if ((res = paging_get_page_index(virtual_address, &page_index)) < 0)
    {
        return res;
    }

    uint32_t entry = directory[table_index];
    uint32_t* table = (uint32_t*)(entry & 0xFFFFF000);  // Address as 20 bit. Full memory is addressable as pages are aligned to 4096=2^12
    table[page_index] = value;
    return 0;
}