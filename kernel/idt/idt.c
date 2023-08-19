#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"

struct idt_descriptor idt[TOTAL_INTERRUPTS];
struct idtr_descriptor idtr;

extern void idt_load(struct idtr_descriptor* ptr);

void idt_zero()
{
    print("Divide by zero error.\n");
}

void idt_set(size_t interrupt_number, void* address)
{
    struct idt_descriptor* descriptor = idt + interrupt_number;
    descriptor->offset_1 = (uint32_t) address & 0x0000FFFF;     // Takes 2 lower bytes
    descriptor->selector = KERNEL_CODE_SELECTOR; 
    descriptor->zero = 0x00;
    descriptor->type_attributes = 0xEE; // First 4 bits are P, DPL and S
    descriptor->offset_2 = (uint32_t) address >> 16;
}

void idt_init()
{
    memset(idt, 0, sizeof(idt));
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t) idt;     // Casting the address pointer to 32 bit integer

    idt_set(0, idt_zero);
    idt_load(&idtr);
}