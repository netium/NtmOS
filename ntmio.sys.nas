; Ntmio.sys
; The first file for the Netium OS

ORG 0xc200
MOV al, 0x13
MOV ah, 0x00
INT 0x10

final:
HLT
JMP final

os_hello_message:
DB 0x0d, 0x0a
DB "This is Netium OS"
DB 0x0d, 0x0a
DB 0