ORG 0x7c00
BITS 16

CODE_ENTRY_OFFSET equ gdt_code - gdt_start
DATA_ENTRY_OFFSET equ gdt_data - gdt_start

; Needed to boot into real hardware
_start:
    jmp short start
    nop
times 33 db 0
 
start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enables Interrupts

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_ENTRY_OFFSET:load32

; GDT
; Paging will be used instead of Segmentation in Protected Mode.
; GDT entries are defined in https://wiki.osdev.org/GDT_Tutorial
; for this scenario.
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0
; Code Segment Descriptor - offset 0x8
gdt_code:           ; CS points to this
    dw 0xffff       ; Segment limit first 0-15 bits
    dw 0            ; Base first 0-15 bits
    db 0            ; Base 16-23 bits
    db 0x9a         ; Access byte
    db 11001111b    ; First 4 bits Limit 48-51, then Flags (52-55)
    db 0            ; Base 24-31 bits

; Data Segment Descriptor - offset 0x10
gdt_data:           ; DS, SS, ES, FS, GS all point to this
    dw 0xffff       ; Segment limit first 0-15 bits
    dw 0            ; Base first 0-15 bits
    db 0            ; Base 16-23 bits
    db 0x92         ; Access byte
    db 11001111b    ; First 4 bits Limit 48-51, then Flags (52-55)
    db 0            ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start
 
[BITS 32]
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


times 510-($ - $$) db 0
dw 0xAA55   ; Boot Signature
