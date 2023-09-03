section .asm
ENABLE_PAGING_BIT equ 0x80000000
global paging_load_directory
global paging_enable_paging
paging_load_directory:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
    mov cr3, eax    ; CR3 contains address to page directory

    pop ebp
    ret
paging_enable_paging:
    push ebp
    mov ebp, ebp

    mov eax, cr0
    or eax, ENABLE_PAGING_BIT
    mov cr0, eax

    pop ebp
    ret