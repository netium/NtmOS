; Ntmio.sys
; The first file for the Netium OS

%include "macro16.nas"

SP_TOP EQU 0x7c00	; To define the address of top of the stack
START_MEM_ADDR EQU 0xc000	; The start address of the code in memory


ORG START_MEM_ADDR

MOV ax, 0x00
MOV ss, ax
MOV ds, ax
MOV sp, SP_TOP
MOV bx, os_hello_message
CALL display_message

reset_drive 0x00

loop:
	hlt
	jmp loop

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

os_hello_message	DB "This is Netium OS", 0x0d, 0x0a, 0x00
