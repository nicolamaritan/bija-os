#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/**
 * @brief Fill a block of memory with a specified value.
 *
 * The memset function fills the first 'n' bytes of the memory block pointed to by 'ptr'
 * with the value of 'c' converted to an unsigned char.
 *
 * @param ptr Pointer to the memory block to be filled.
 * @param c Value to be set. It is passed as an integer, but only the low-order byte is used.
 * @param n Number of bytes to be filled with the specified value.
 * @return A pointer to the memory block 'ptr'.
 *
 * @note This function does not perform any kind of boundary checks, so ensure that the
 *       memory block pointed to by 'ptr' has at least 'n' valid bytes.
 *
 * @warning Using this function to set memory to non-zero values is unsafe for sensitive
 *          data as it may leave data remnants in memory.
 *
 * @see memcpy(), memmove()
 */
void* memset(void* ptr, int c, size_t n);

#endif