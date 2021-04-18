; For C calling conventions, refer to: https://www.csee.umbc.edu/~chang/cs313.s02/stack.shtml

GLOBAL _io_hlt
GLOBAL _io_cli
GLOBAL _io_out8
GLOBAL _io_out16
GLOBAL _io_out32
GLOBAL _io_in8
GLOBAL _io_in16
GLOBAL _io_in32

GLOBAL _get_eip

GLOBAL _enable_interrupt_and_halt

GLOBAL _set_tr

GLOBAL _switch_task

GLOBAL _panic
GLOBAL _putchar

GLOBAL _atom_inc

section .text

_io_out8:
	MOV edx, [esp+4]
	MOV eax, [esp+8]
	OUT dx, al
	RET

_io_out16:
	MOV edx, [esp+4]
	MOV eax, [esp+8]
	OUT dx, ax
	RET

_io_out32:
	MOV edx, [esp+4]
	MOV eax, [esp+8]
	OUT dx, eax
	RET 

_io_in8:
	MOV edx, [esp+4]
	MOV eax, 0x0
	IN al, dx
	RET

_io_in16:
	MOV edx, [esp+4]
	MOV eax, 0x0
	IN ax, dx
	RET

_io_in32:
	MOV edx, [esp+4]
	MOV eax, 0x0
	IN eax, dx
	RET

_get_eip:
	MOV eax, [esp]
	RET

_set_tr:
	LTR [esp + 4]
	RET

_switch_task:
	JMP FAR [esp + 4]
	RET

_panic:	
.panic_loop:
	HLT
	JMP .panic_loop

_putchar:
	MOV eax, [esp + 4]
	MOV ecx, 0xB8000
	MOV BYTE[ecx], al
	INC ecx
	MOV BYTE[ecx], 0x0B
	RET