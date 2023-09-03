#include "kernel.h"
#include "idt/idt.h"
#include "tty/tty.h"
#include "io/io.h"
#include "pic/pic.h"
#include "memory/paging/paging.h"
#include "memory/heap/kernel_heap.h"
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

static page_directory* kernel_directory;

void kernel_main()
{
    terminal_initialize();
    print("Terminal initialized.\n");

    kernel_heap_init();
    print("Kernel Heap initialized.\n");

    kernel_directory = paging_get_directory(PAGING_IS_WRITABLE | PAGING_IS_PRESENT);
    paging_switch_current_directory(kernel_directory);
    paging_enable_paging();
    print("Paging enabled. Switched to Kernel page directory.\n");

    idt_init();
    pic_remap(OFFSET_MASTER_PIC, OFFSET_SLAVE_PIC);
    print("IDT initialized. PIC remapped.\n");

    enable_interrupts();
    print("Interrupts enabled.\n");


}