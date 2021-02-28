; Ntmio.sys
; The first file for the Netium OS

ORG 0xc200
MOV al, 0x13
MOV ah, 0x00
INT 0x10

final:
HLT
JMP final