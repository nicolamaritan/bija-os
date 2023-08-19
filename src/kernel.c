#include "kernel.h"
#include "idt/idt.h"
#include <stdint.h>
#include <stddef.h>

uint16_t* terminal_buffer = 0;
size_t terminal_row = 0;
size_t terminal_column = 0;

uint16_t vga_entry(char c, uint8_t color)
{
    return (color << 8) | c;
}

void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_initialize()
{
    terminal_buffer = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            terminal_put_entry_at(' ', 0, x, y);
        }
    }

}

void terminal_putchar(char c, uint8_t color)
{
    if (c == '\n')
    {
        terminal_column = 0;
        terminal_row ++;
        return;
    }

	terminal_put_entry_at(c, color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) 
    {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
        {
			terminal_row = 0;
        }
	}
} 


size_t strlen(const char* str)
{
    size_t len;
    for (len = 0; str[len]; len++);
    return len;
}

void print(const char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        terminal_putchar(str[i], 15);
    }
}

extern void divide_by_zero();
void kernel_main()
{
    terminal_initialize();
    print("Hello World!\n");

    idt_init();

    divide_by_zero();   // Testing
}