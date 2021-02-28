; ntmOS
; TAB=4

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
MOV si, message

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

MOV ah, 0x02
MOV al, 1
MOV bx, 0
MOV dl, 0x00
MOV 0x13
JC disk_load_error

final:
HLT
JMP final

disk_load_error:
MOV si, error_message

; Display the bootloading message
.error_diplay:
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

message:
DB 0x0d, 0x0a
DB "NtM-Bootloader"
DB 0x0d, 0x0a
DB "For NetiumOS"
DB 0x0a
DB 0

DB 0x1fe-($-$$) DUP 0
DB 0x55, 0xaa

DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
DB 4600 DUP 0 

DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
DB 1469432 DUP 0