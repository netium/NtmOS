; ntmOS
; TAB=4

CYLS EQU 10
SP_BOTTOM EQU 0x7BFF
MBR_MEM_ADDR EQU 0x7c00
BOOT_DRIVE EQU 0x00
BL2_BASE_MEM EQU 0x0820

%include "macro16.nas"

    ;ORG 0x7c00
    ORG MBR_MEM_ADDR

    JMP start   ; Sector offset:0x000, short jump with (0xEB 0x?? 0x90)
    DB 0x90
    DB "NETIUMOS"   ; Sector offset:0x003, OEM Name, 8bytes

    ; DOS 4.0 EBPB, refer to: https://en.wikipedia.org/wiki/BIOS_parameter_block
    DW 512  ; Sector offset:0x00B, Bytes per logical sector
    DB 1    ; Sector offset:0x00D, Logical sectors per cluster
    DW 1    ; Sector offset:0x00E, Reserved logical sectors
    DB 2    ; Sector offset:0x010, Number of FATs
    DW 224  ; Sector offset:0x011, Root directory entries
    DW 2880 ; Sector offset:0x013, Total logical sectors
    DB 0xf0 ; Sector offset:0x015, Media descriptor
    DW 9    ; Sector offset:0x016, Logical sectors per FAT
    DW 18   ; Sector offset:0x018, Physical sectors per track
    DW 2    ; Sector offset:0x01A, Number of heads
    DD 0    ; Sector offset:0x01C, Hidden sectors
    DD 2880 ; Sector offset:0x020, Large total logical sectors
    DB 0    ; Sector offset:0x024, Physical drive number
    DB 0    ; Sector offset:0x025, Flags etc.
    DB 0x29 ; Sector offset:0x026, Extended boot signature
    DD 0xFFFFFFFF       ; Sector offset:0x27,   Volume serial number
    DB "NETIUM-OS  "    ; Sector offset:0x02B, Volume label
    DB "FAT12   "       ; Sector offset:0x36,   File-system type
    DB 18 DUP (0)
    ;RESB 18

start:
    MOV ax, 0
    MOV ss, ax
    MOV sp, MBR_MEM_ADDR
    MOV ds, ax
    MOV bx, bootloader_greeting_message
    CALL display_message
    reset_drive BOOT_DRIVE
;   Shall check whether the reset is successful here

; Load disk section
load_boot_module:
    MOV ax, BL2_BASE_MEM
    MOV es, ax
    MOV ch, 0       ; Cylinder
    MOV dh, 0       ; Head
    MOV cl, 2       ; Sector Number

read_loop:
    MOV si, 0

; Implement the retry-able read, will retry 3 times
.retry_read:
    MOV ah, 0x02
    MOV al, 1       ; # of Sectors to read
    MOV bx, 0       ; ES:BX - Buffer address pointer
    MOV dl, BOOT_DRIVE  ; Drive
    INT 0x13
    JNC next
    ADD si, 1
    CMP si, 3
    JAE disk_load_error
    ; To reset disk and then retry
    reset_drive BOOT_DRIVE
    JMP .retry_read

next:
    MOV ax, es
    ADD ax, 0x0020      ; Increase segment by 0x20, so increase the address by 512 bytes
    MOV es, ax
    ADD cl, 1
    CMP cl, 18
    JBE read_loop
    MOV cl, 1
    add dh, 1
    CMP dh, 2
    JB read_loop
    MOV dh, 0
    ADD ch, 1
    CMP ch, CYLS
    JB read_loop

    MOV [0x0ff0], ch    ; Copy the # of boot sectors to 0x0ff0

    JMP 0xc200      ; Load complete, jump to the loaded code, never come back

; Display message function
; Parameters:
;   - BX: The start index address of the message, the message shall be end with '\0'
; Return:
;   - No return value
display_message:
    MOV si, bx
.display_loop:
    MOV al, [si]
    ADD si, 1
    CMP al, 0
    JE .ret
    MOV ah, 0x0e
    MOV bx, 15
    INT 0x10
    JMP .display_loop
.ret:
    RET

disk_load_error:
    MOV bx, error_message
    CALL display_message
    JMP final

final:
    hlt
    jmp final

i_am_here_message: ; Fore debug output
    DB "I am here", 0x0d, 0x0a, 0x0

error_message:
    DB "Error", 0x0d, 0x0a, 0x0

read_message:
    DB "Read...", 0x0d, 0x0a, 0x0

bootloader_greeting_message:
    DB "NtM-Bootloader 0.1", 0x0d, 0x0a, 0x0

    DB 0x1fe-($-$$) DUP (0)
    ;RESB 0x1fe-($-$$)
    DB 0x55, 0xaa   ; Sector offset:0x1FE, Boot sector signature (0x55, 0xAA)

    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 4600 DUP (0)

    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 1469432 DUP (0)
