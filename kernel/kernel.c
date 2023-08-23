#include "kernel.h"
#include "idt/idt.h"
#include "tty/tty.h"
#include "io/io.h"
#include "pic/pic.h"
#include <stdint.h>
#include <stddef.h>

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

void kernel_main()
{
    terminal_initialize();
    print("Hello World!\n");

    idt_init();
    PIC_remap(0x20, 0x28);

}