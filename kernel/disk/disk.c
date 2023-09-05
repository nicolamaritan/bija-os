#include "disk.h"
#include "io/io.h"

#define MASTER_DRIVE 0xE0
#define WORDS_PER_SECTOR 256

int disk_read_sector(size_t lba, size_t n, void* buffer)
{
    // Analogous to simple disk driver in boot.asm
    outb(0x1F6, ((lba >> 24) & 0xFF) | MASTER_DRIVE);
    outb(0x1F2, n);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, 0x20);

    uint16_t* word_buffer = (uint16_t*)buffer;
    for (size_t i = 0; i < n; i++)
    {
        // Wait for disk
        while (!(insb(0x1F7) & 0x08)); // Checks the third bit of the status register. This bit indicates whether the disk is busy or not. If it is 1, the disk is busy; if it is 0, the disk is ready.

        // Read one sector
        for (size_t j = 0; j < WORDS_PER_SECTOR; j++)
        {
            word_buffer[i * WORDS_PER_SECTOR + j] = insw(0x1F0);
        }
    }

    return 0;
}