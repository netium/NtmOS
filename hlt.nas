;[FORMAT "WCOFF"]
[BITS 32]

;[FILE "hlt.nas"]

GLOBAL _io_hlt

;[SECTION .text]

section .data
section .text

_io_hlt:
	; HLT ;	This command will cause 'sgemental error' if this function is called in the 32bit userspace as it's privilleged command
	RET
