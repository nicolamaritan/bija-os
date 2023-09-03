#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>


#define PAGING_CACHE_DISABLED   0x10
#define PAGING_WRITE_THROUGH    0X08
#define PAGING_ACCESS_FROM_ALL  0x04    // Accessable from all rings
#define PAGING_IS_WRITABLE      0x02
#define PAGING_IS_PRESENT       0x01

#define PAGING_PAGES_PER_TABLE      1024
#define PAGING_TABLES_PER_DIRECTORY 1024

#define PAGING_PAGE_SIZE 4096

typedef uint32_t page_directory;

struct paging_4gb_chunk
{
    page_directory* directory;  // 4gb of addressable memory
};

struct paging_4gb_chunk* paging_init_chunk(uint8_t flags);
void paging_enable_paging();
void paging_switch_current_directory(page_directory* directory);

#endif