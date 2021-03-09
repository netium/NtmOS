; For C calling conventions, refer to: https://www.csee.umbc.edu/~chang/cs313.s02/stack.shtml

GLOBAL _io_hlt
GLOBAL _io_out8
GLOBAL _io_out16
GLOBAL _io_out32
GLOBAL _io_in8
GLOBAL _io_in16
GLOBAL _io_in32
GLOBAL _get_eflags
GLOBAL _set_eflags
GLOBAL _load_gdt
GLOBAL _load_idt
GLOBAL _disable_interrupt
GLOBAL _enable_interrupt

GLOBAL _get_eip

section .text

_io_hlt:
	HLT ;	This command will cause 'sgemental error' if this function is called in the 32bit userspace as it's privilleged command
	RET

_io_cli:
	CLI
	RET

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

_get_eflags:
	PUSHFD
	POP eax
	RET

_set_eflags:
	MOV eax, [esp+4]
	PUSH eax
	POPFD
	RET

_get_eip:
	MOV eax, [esp]
	RET

_load_gdt:
	LGDT [esp+4]
	RET

_load_idt:
	LIDT [esp+4]
	RET

_disable_interrupt:
	CLI
	RET

_enable_interrupt:
	STI
	RET
