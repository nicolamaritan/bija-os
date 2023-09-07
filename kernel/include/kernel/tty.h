#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

/**
 * Represents a VGA text mode entry with character and color information.
 *
 * @param c     The character to display.
 * @param color The color information for the character.
 * @return      The VGA text mode entry composed of character and color.
 */
uint16_t vga_entry(char c, uint8_t color);

/**
 * Places a character with a specific color at the specified position in the terminal buffer.
 *
 * @param c     The character to display.
 * @param color The color information for the character.
 * @param x     The column position in the terminal.
 * @param y     The row position in the terminal.
 */
void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y);

/**
 * Initializes the VGA terminal by clearing the buffer and setting the initial row and column.
 */
void terminal_initialize();

/**
 * Displays a character with a specific color in the VGA terminal at the current cursor position.
 * Handles newline characters by moving the cursor to the beginning of the next line.
 *
 * @param c     The character to display.
 * @param color The color information for the character.
 */
void terminal_putchar(char c, uint8_t color);


#endif