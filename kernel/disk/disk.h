#ifndef DISK_H
#define DISK_H

#include <stddef.h>

/**
 * Reads one or more sectors from a disk using the Logical Block Address (LBA) method.
 *
 * This function sends commands to a disk controller to read specified sectors from a disk
 * and stores the data in the provided buffer.
 *
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
int disk_read_sector(size_t lba, size_t n, void* buffer);

#endif