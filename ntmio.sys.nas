; Ntmio.sys
; The first file for the Netium OS

SP_TOP EQU 0x7c00	; To define the address of top of the stack
START_MEM_ADDR EQU 0xc200	; The start address of the code in memory

BOTPAK EQU 0x00280000
DSKCAC EQU 0100100000
DSKCAC0 EQU 0x00008000

CYLS EQU 0x0ff0
LEDS EQU 0x0ff1
VMODE EQU 0x0ff2
SCRNX EQU 0x0ff4
SCRNY EQU 0x0ff6
VRAM EQU 0x0ff8

global _start
extern boot_main

section .text

[bits 16]
; ORG START_MEM_ADDR
_start:
MOV ax, 0x00
MOV ss, ax
MOV ds, ax
MOV sp, SP_TOP
MOV bx, os_hello_message
CALL display_message
JMP continue

loop:
	hlt
	jmp loop

; Display message function
; Parameters:
;   - BX: The start index address of the message, the message shall be end with '\0'
; Return:
;   - No return value
display_message:
    MOV si, bx
.display:
    MOV al, [si]
    ADD si, 1
    CMP al, 0
    JE .return
    MOV ah, 0x0e
    MOV bx, 15
    INT 0x10
    JMP .display
.return:
    RET

os_hello_message:
DB "This is Netium OS", 0x0d, 0x0a, 0x00

continue:
MOV al, 0x13
MOV ah, 0x00
INT 0x10
MOV BYTE [VMODE],8
MOV WORD [SCRNX],320
MOV WORD [SCRNY],200
MOV DWORD [VRAM], 0x000a0000

MOV ah, 0x02
INT 0x16
MOV [LEDS], AL

MOV al, 0xff
OUT 0x21, al
NOP
OUT 0xa1, AL

CLI

CALL waitkbdout
MOV al, 0xd1
OUT 0x64, al
CALL waitkbdout
MOV al, 0xdf
OUT 0x60, al
CALL waitkbdout

;[INSTRSET "i486p"]
LGDT [GDTR0]
MOV eax, CR0
AND EAX, 0x7fffffff
OR EAX, 0x00000001
MOV cr0, eax
JMP pipelineflush
pipelineflush:
MOV ax, 1*8
MOV ds, ax
MOV es, ax
MOV fs, ax
MOV gs, ax
MOV ss, ax
MOV esi, bootpack
MOV edi, BOTPAK
MOV ecx, 512*1024/4
CALL memcpy

		MOV		ESI,0x7c00
		MOV		EDI,DSKCAC
		MOV		ECX,512/4
		CALL	memcpy

		MOV		ESI,DSKCAC0+512	
		MOV		EDI,DSKCAC+512
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4
		SUB		ECX,512/4
		CALL	memcpy

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3
		SHR		ECX,2
		JZ		skip
		MOV		ESI,[EBX+20]
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]
		JMP		DWORD 2*8:0x0000001b

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy
		RET

		ALIGNB	16
GDT0:
		DB  	8 dup (0)
		DW		0xffff,0x0000,0x9200,0x00cf	
		DW		0xffff,0x0000,0x9a28,0x0047

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
    ;CALL boot_main
	hlt
	jmp bootpack