; ntmOS
; TAB=4

CYLS equ 10
ORG 0x7c00

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
DB 18 DUP 0

start:
MOV ax, 0
MOV ss, ax
MOV sp, 0x7c00
MOV ds, ax
MOV es, ax
MOV si, bootloader_greeting_message

; Display the bootloading message
display:
MOV al, [si]
ADD si, 1
CMP al, 0
JE final
MOV ah, 0x0e
MOV bx, 15
INT 0x10
JMP display

; Load disk secion
MOV ax, 0x0820
MOV es, ax
MOV ch, 0
MOV dh, 0
MOV cl, 2

read_loop:
MOV si, 0

; Implement the retry-able read, will retry 3 times
retry_read:
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
MOV ah,0
MOV dl, 0
INT 0x13
JMP retry_read

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

final:
HLT
JMP final

disk_load_error:
MOV si, error_message

; Display the bootloading message
.error_display:
MOV al, [si]
ADD si, 1
CMP al, 0
JE final
MOV ah, 0x0e
MOV bx, 15
INT 0x10
JMP .error_display

error_message:
DB 0x0d, 0x0a
DB "Load error"
DB 0x0d, 0x0a
DB 0

bootloader_greeting_message:
DB 0x0d, 0x0a
DB "NtM-Bootloader"
DB 0x0d, 0x0a
DB "For NetiumOS"
DB 0x0a
DB 0



DB 0x1fe-($-$$) DUP 0
DB 0x55, 0xaa

;DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
;DB 4600 DUP 0 

;DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
;DB 1469432 DUP 0