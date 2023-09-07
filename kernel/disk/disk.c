#include <kernel/disk.h>
#include <kernel/io.h>
#include <kernel/memory/memory.h>
#include <kernel/config.h>
#include <kernel/status.h>

#define MASTER_DRIVE 0xE0
#define WORDS_PER_SECTOR 256

#define READ_COMMAND 0x20
#define WRITE_COMMAND 0x30

static struct disk primary_disk;

int disk_read_sector(size_t lba, size_t n, void* buffer)
{
    // Analogous to simple disk driver in boot.asm
    outb(0x1F6, ((lba >> 24) & 0xFF) | MASTER_DRIVE);
    outb(0x1F2, n);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, READ_COMMAND);

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

int disk_write_sector(size_t lba, size_t n, void* buffer)
{
    outb(0x1F6, ((lba >> 24) & 0xFF) | MASTER_DRIVE);
    outb(0x1F2, n);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, WRITE_COMMAND);

    uint16_t* word_buffer = (uint16_t*)buffer;
    for (size_t i = 0; i < n; i++)
    {
        // Wait for disk
        while (!(insb(0x1F7) & 0x08)); // Checks the third bit of the status register. This bit indicates whether the disk is busy or not. If it is 1, the disk is busy; if it is 0, the disk is ready.

        // Read one sector
        for (size_t j = 0; j < WORDS_PER_SECTOR; j++)
        {
            outw(0x1F0, word_buffer[i * WORDS_PER_SECTOR + j]);
        }
    }

    return 0;
}

void disk_init()
{
    memset(&primary_disk, 0, sizeof(primary_disk));
    primary_disk.type = DISK_TYPE_REAL;
    primary_disk.sector_size = DISK_SECTOR_SIZE;
}

struct disk* disk_get(size_t index)
{
    if (index != 0)
        return NULL;
    return &primary_disk;
}

int disk_read_block(struct disk* disk, size_t lba, size_t n, void* buffer)
{
    if (disk != &primary_disk)
    {
        return -EIO;
    }
    return disk_read_sector(lba, n, buffer);
}

int disk_write_block(struct disk* disk, size_t lba, size_t n, void* buffer)
{
    if (disk != &primary_disk)
    {
        return -EIO;
    }
    return disk_write_sector(lba, n, buffer);   
}