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

/**
 * @brief Creates a new page directory with page tables and returns a pointer to it.
 *
 * @param flags Flags to apply to the directory and table entries.
 * @return A pointer to the newly created page directory if successful, or NULL if there's not enough memory.
 */
uint32_t* paging_make_directory(uint8_t flags);

/**
 * @brief Enables the paging.
 * 
 * @warning Invoking `paging_switch_current_directory` with a valid page directory before this method
 * is required to avoid kernel panics.
*/
void paging_enable_paging();

/**
 * @brief Switches the current page directory to the specified directory.
 *
 * @param directory Pointer to the new page directory to switch to.
 */
void paging_switch_current_directory(uint32_t* directory);

/**
 * @brief Checks if a given address is aligned to a page boundary.
 *
 * @param addr Pointer to the address to be checked.
 * @return 0 if and only if the address is not aligned.
 */
int paging_is_aligned(void* addr);

/**
 * @brief Gets the table index for a given virtual address.
 *
 * This function calculates the table index for a given virtual address based on
 * the page size and the number of pages per table.
 *
 * @param virtual_address Pointer to the virtual address.
 * @param table_index_out Pointer to a size_t variable to store the table index.
 * @return 0 on success, -EINVARG if the address is not aligned.
 */
int paging_set_page_table_entry(uint32_t* directory, void* virtual_address, uint32_t value);

#endif