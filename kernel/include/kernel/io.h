#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
 * Read a byte from the specified I/O port.
 *
 * @param port The I/O port from which to read the byte.
 * @return The byte read from the specified port.
 */
uint8_t insb(uint16_t port);

/**
 * Read a word (16 bits) from the specified I/O port.
 *
 * @param port The I/O port from which to read the word.
 * @return The word read from the specified port.
 */
uint16_t insw(uint16_t port);

/**
 * Write a byte to the specified I/O port.
 *
 * @param port The I/O port to which the byte will be written.
 * @param val The byte value to write to the port.
 */
void outb(uint16_t port, uint8_t val);

/**
 * Write a word (16 bits) to the specified I/O port.
 *
 * @param port The I/O port to which the word will be written.
 * @param val The word value to write to the port.
 */
void outw(uint16_t port, uint16_t val);

/**
 * Performs a brief delay to allow I/O operations to complete.
 * This is often used to synchronize with hardware after I/O operations.
 */
void io_wait();

#endif