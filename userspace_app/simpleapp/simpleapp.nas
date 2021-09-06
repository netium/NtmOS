section .text
global _start

_start:
.loop:
	xor ebx, ebx
	inc ebx
	mov eax, 01h
	int 80h
jmp short .loop