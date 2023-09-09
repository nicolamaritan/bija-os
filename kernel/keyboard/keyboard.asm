section .asm

global keyboard_irq

keyboard_irq:
    cli
    pushad
    call keyboard_irq_handler
    popad
    sti
    iret

extern keyboard_irq_handler