; ntmOS
; TAB=4

CYLS EQU 10
SP_BOTTOM EQU 0x7BFF
MBR_MEM_ADDR EQU 0x7c00

    ;ORG 0x7c00
    ORG MBR_MEM_ADDR

    JMP start
    DB 0x90
    DB "NETIUMOS"
    DW 512
    DB 1
    DW 1
    DB 2
    DW 224
    DW 2880
    DB 0xf0
    DW 9
    DW 18
    DW 2
    DD 0
    DD 2880
    DB 0, 0, 0x29
    DD 0xFFFFFFFF
    DB "NETIUM-OS  "
    DB "FAT12   "
    ;DB 18 DUP 0
    RESB 18

start:
    MOV ax, 0
    MOV ss, ax
    MOV sp, MBR_MEM_ADDR
    MOV ds, ax
    MOV bx, bootloader_greeting_message
    CALL display_message

;MOV si, bootloader_greeting_message

; Load disk secion
load_boot_module:
    MOV ax, 0x0820
    MOV es, ax
    MOV ch, 0
    MOV dh, 0
    MOV cl, 2

read_loop:
    MOV si, 0

; Implement the retry-able read, will retry 3 times
.retry_read:
    MOV ah, 0x02
    MOV al, 1
    MOV bx, 0
    MOV dl, 0x00
    INT 0x13
    JNC next
    ADD si, 1
    CMP si, 3
    JAE disk_load_error
    ; To reset disk and then retry
    MOV ah,0x0
    MOV dl, 0x0
    INT 0x13
    JMP .retry_read

next:
    MOV ax, es
    ADD ax, 0x0020
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

final: ; Should never reach out to here as the it will jump to the 0xc200 already
    HLT
    JMP final

; Display message function
; Parameters:
;   - BX: The start index address of the message, the message shall be end with '\0'
; Return:
;   - No return value
display_message:
    MOV si, bx
.display:
    MOV al, [si]
    ADD si, 1
    CMP al, 0
    JE .return
    MOV ah, 0x0e
    MOV bx, 15
    INT 0x10
    JMP .display
.return:
    RET

disk_load_error:
    MOV bx, error_message
    CALL display_message
    JMP final

i_am_here_message: ; Fore debug output
    DB "I am here", 0x0d, 0x0a, 0x0

error_message:
    DB "Error", 0x0d, 0x0a, 0x0

read_message:
    DB "Read...", 0x0d, 0x0a, 0x0

bootloader_greeting_message:
    DB "NtM-Bootloader 0.1", 0x0d, 0x0a, 0x0

    ;DB 0x1fe-($-$$) DUP 0
    RESB 0x1fe-($-$$)
    DB 0x55, 0xaa

    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 4600 DUP 0 

    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 1469432 DUP 0