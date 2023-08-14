[BITS 32]
CODE_ENTRY_OFFSET equ 0x08
DATA_ENTRY_OFFSET equ 0x10
load32:
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

    jmp $