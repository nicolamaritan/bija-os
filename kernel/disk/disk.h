#ifndef DISK_H
#define DISK_H

#include <stddef.h>

typedef size_t disk_type;
#define DISK_TYPE_REAL 0

struct disk
{
    disk_type type;
    size_t sector_size;
};

/**
 * Reads one or more sectors from a disk using the Logical Block Address (LBA) method.
 *
 * This function sends commands to a disk controller to read specified sectors from a disk
 * and stores the data in the provided buffer.
 *
 * @param disk The logical disk to read from
 * @param lba The starting Logical Block Address (LBA) of the sectors to be read.
 * @param n   The number of sectors to read.
 * @param buffer A pointer to the buffer where the read data will be stored.
 *
 * @return 0 if the read operation is successful, or an error code if it fails.
 *
 * @note The function assumes that the disk controller is set up correctly and that the
 *       necessary hardware I/O ports are properly configured.
 * @note The function uses the ATA/ATAPI command set for disk operations.
 * @note It is the caller's responsibility to allocate and manage the memory for the buffer.
 * @note This function may block until the disk is ready for reading.
 */
int disk_read_block(struct disk* disk, size_t lba, size_t n, void* buffer);

int disk_write_block(struct disk* disk, size_t lba, size_t n, void* buffer);

/**
 * @brief Initializes the disks in the system.
*/
void disk_init();

/**
 * @brief Returns the pointer of the disk specified by the given index.
 * 
 * @return The pointer of the specified index, NULL if the index is invalid.
*/
struct disk* disk_get(size_t index);

#endif