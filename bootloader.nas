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

display:
MOV al, [si]
ADD si, 1
CMP al, 0
JE final
MOV ah, 0x0e
MOV bx, 15
INT 0x10
JMP display

final:
HLT
JMP final

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