section .asm
global insb
global insw
global outb
global outw
global io_wait
insb:
    push ebp
    mov ebp, esp

    ; eax is the return value when working with asm + C
    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; Move the first parameter (port) into edx
    in al, dx           ; Result of input is stored in al, which is part of eax

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    ; eax is the return value when working with asm + C
    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; Move the first parameter (port) into edx
    in ax, dx           ; Result of input is stored in ax, which is part of eax

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8]
    mov eax, [ebp+12]
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8]
    mov eax, [ebp+12]
    out dx, ax

    pop ebp
    ret

io_wait:
    push ebp
    mov ebp, esp

    mov dx, 0x80
    mov al, 0x00
    out dx, al

    pop ebp
    ret