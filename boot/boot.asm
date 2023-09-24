ORG 0x7c00
BITS 16

CODE_ENTRY_OFFSET equ gdt_code - gdt_start
DATA_ENTRY_OFFSET equ gdt_data - gdt_start
KERNEL_LOADING_ADDRESS equ 0x100000

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
    jmp CODE_ENTRY_OFFSET:load32    ; Load CS with CODE_ENTRY_OFFSET (0x08) and jumps to load32

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
    mov eax, 1
    mov ecx, 100
    mov edi, KERNEL_LOADING_ADDRESS
    call ata_lba_read
    jmp CODE_ENTRY_OFFSET:KERNEL_LOADING_ADDRESS

;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param ECX  Number of sectors to read
; @param EDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
ata_lba_read:
    mov ebx, eax ; Backup the LBA

    ; Send the lowest 8 bits of the LBA to the HD controller
    shr eax, 24     ; Get bit 24 - 27 in al
    or eax, 0xE0    ; Select Master Drive
    mov dx, 0x1F6   ; Port to send drive and bit 24 - 27 of LBA
    out dx, al

    ; Send the number of sectors to read
    mov eax, ecx    ; Get number of sectors from ECX
    mov dx, 0x1F2   ; Port to send number of sectors
    out dx, al

    mov eax, ebx    ; Restore LBA backup
    mov dx, 0x1F3   ; Port to send bit 0 - 7 of LBA
    out dx, al

    mov dx, 0x1F4   ; Port to send bit 8 - 15 of LBA
    mov eax, ebx
    shr eax, 8
    out dx, al

    mov dx, 0x1F5   ; Port to send bit 16 - 23 of LBA
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20    ; Read with retry
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1F7
    in al, dx
    test al, 8      ; Checks if the disk can service
    jz .try_again   ; Jump if the test fails

    mov ecx, 256
    mov dx, 0x1F0
    rep insw        ; Repeat exc (256) times: reads 16 bits from the port 0x1F0 and stores it in ES:DI (0x00:0x100000)

    pop ecx
    loop .next_sector

    ret



times 510-($ - $$) db 0
dw 0xAA55   ; Boot Signature
