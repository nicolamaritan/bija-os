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

uint32_t* paging_get_directory(uint8_t flags);
void paging_enable_paging();
void paging_switch_current_directory(uint32_t* directory);

#endif