[BITS 32]
global _start

extern kernel_main

CODE_ENTRY_OFFSET equ 0x08
DATA_ENTRY_OFFSET equ 0x10
_start:
    mov ax, DATA_ENTRY_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al

    call kernel_main
    jmp $

times 512-($ - $$) db 0