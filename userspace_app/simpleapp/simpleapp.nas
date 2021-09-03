section .text
global _start

_start:
.loop:
	xor eax, eax
	inc eax
jmp short .loop