#include <kernel/memory/heap/heap.h>
#include <kernel/config.h>
#include <kernel/status.h>
#include <kernel/memory/memory.h>

/**
 * @brief Validates the alignment of a pointer within the heap.
 *
 * @param ptr Pointer to validate.
 * @return 1 if the pointer is correctly aligned, 0 otherwise.
 */
static int heap_validate_alignment(void* ptr)
{
    return ((size_t)ptr % HEAP_BLOCK_SIZE) == 0;
}

/**
 * @brief Validates the heap table and its entries.
 *
 * @param ptr Pointer to the start of the heap.
 * @param end Pointer to the end of the heap.
 * @param table Pointer to the heap table.
 * @return 0 if the table is valid, -EINVARG if it's not.
 */
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    size_t table_size = (size_t)(end - ptr);
    size_t table_entries_number = table_size / HEAP_BLOCK_SIZE;
    if (table->entries_number != table_entries_number)
    {
        return -EINVARG;
    }

    return 0;
}

/**
 * @brief Finds the starting block for allocating a contiguous sequence of blocks in the heap.
 *
 * @param heap Pointer to the heap.
 * @param blocks_number Number of blocks to allocate.
 * @return The index of the starting block if available, or ENOMEM if there's not enough memory.
 */
static int heap_get_start_block(struct heap* heap, size_t blocks_number)
{
    struct heap_table* table = heap->table;
    size_t count = 0;
    int start_block = -1;
    for (size_t i = 0; i < table->entries_number; i++)
    {
        if (table->entries[i] != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            count = 0;
            start_block = -1;
            continue;
        }

        start_block = start_block == -1 ? i : start_block;  // Set start_block to i only if it was unset
        count++;
        if (count == blocks_number)
        {
            break;
        }
    }
    
    return start_block >= 0 ? start_block : -ENOMEM; 
}

/**
 * @brief Marks a sequence of blocks in the heap as taken.
 *
 * @param heap Pointer to the heap.
 * @param start_block Index of the starting block.
 * @param blocks_number Number of blocks to mark.
 */
static void heap_mark_taken_blocks(struct heap* heap, size_t start_block, size_t blocks_number)
{
    struct heap_table* table = heap->table;
    table->entries[start_block] = HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    if (blocks_number == 1)
    {
        return;
    }

    // Has next only if the blocks_number is greater than one
    table->entries[start_block] |= HEAP_BLOCK_HAS_NEXT;

    // Marks each block but the first and the last with HEAP_BLOCK_HAS_NEXT | HEAP_BLOCK_TABLE_ENTRY_TAKEN
    for (size_t offset_block = 1; offset_block < blocks_number - 1; offset_block++)
    {
        table->entries[start_block + offset_block] = HEAP_BLOCK_HAS_NEXT | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    }

    // The last block has no next
    table->entries[start_block + blocks_number - 1] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
}

/**
 * @brief Marks a sequence of blocks in the heap as free.
 *
 * @param heap Pointer to the heap.
 * @param start_block Index of the starting block.
 */
static void heap_mark_free_blocks(struct heap* heap, size_t start_block)
{
    struct heap_table* table = heap->table;
    for (size_t i = start_block; i < table->entries_number; i++)
    {
        HEAP_BLOCK_TABLE_ENTRY old_entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if (!(old_entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

/**
 * @brief Aligns a value to the nearest multiple of HEAP_BLOCK_SIZE.
 *
 * @param val Value to be aligned.
 * @return The aligned value.
 */
static uint32_t heap_block_align(uint32_t val)
{
    if (val % HEAP_BLOCK_SIZE == 0)
    {
        return val;
    }

    return (val / HEAP_BLOCK_SIZE + 1) * HEAP_BLOCK_SIZE;
}

/**
 * @brief Converts a block index to its corresponding address within the heap.
 *
 * @param heap Pointer to the heap.
 * @param block Block index.
 * @return The address corresponding to the block.
 */
static void* heap_block_to_address(struct heap* heap, size_t block)
{
    return heap->start_address + (block * HEAP_BLOCK_SIZE);
}

/**
 * @brief Converts an address to the corresponding block index within the heap.
 *
 * @param heap Pointer to the heap.
 * @param address Address to convert.
 * @return The block index corresponding to the address.
 */
static size_t heap_address_to_block(struct heap* heap, void* address)
{
    return ((size_t)(address - heap->start_address)) / HEAP_BLOCK_SIZE;
}

/**
 * @brief Allocates a contiguous sequence of blocks in the heap.
 *
 * @param heap Pointer to the heap.
 * @param blocks_number Number of blocks to allocate.
 * @return A pointer to the first block if successful, or NULL if there's not enough memory.
 */
static void* heap_malloc_blocks(struct heap* heap, size_t blocks_number)
{
    void* address = 0;
    int start_block = heap_get_start_block(heap, blocks_number);
    if (start_block < 0)
    {
        return NULL;
    }
    address = heap_block_to_address(heap, start_block);
    heap_mark_taken_blocks(heap, start_block, blocks_number);
    return address;
}

void* heap_malloc(struct heap* heap, size_t size)
{
    size_t aligned_size = heap_block_align(size);
    size_t blocks_to_allocate = aligned_size / HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap, blocks_to_allocate);
}

void heap_free(struct heap* heap, void* ptr)
{
    size_t block = heap_address_to_block(heap, ptr);
    heap_mark_free_blocks(heap, block);
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    int res = 0;
    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        return -EINVARG;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->start_address = ptr;
    heap->table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < 0)
    {
        return res;
    }

    // Initialize table
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->entries_number;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

    return 0;
}