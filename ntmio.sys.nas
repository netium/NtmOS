; Author: Bo Zhou (zb.public@outlook.com)
; Under GPLv2.0 lincense

%include "macro16.nas"

SP_TOP EQU 0x7c00	; To define the address of top of the stack
START_MEM_ADDR EQU 0x07E00	; The start address of the code in memory
NUM_OF_HEAD EQU 2
SECTORS_PER_TRACK EQU 18
SECTORS_PER_FAT EQU 9
SECTORS_FOR_ROOT_DIR EQU (224 * 32) / 512
BOOT_DRIVE EQU 0x00

KERNEL_FILE_BASE_MEM_ADDR EQU 0xa000

MBR_BASE_MEM_ADDR EQU 0x07C00
FAT_BASE_MEM_ADDR EQU 0x500
ROOT_DIR_BASE_MEM_ADDR EQU FAT_BASE_MEM_ADDR + (9 * 0x200)

ORG START_MEM_ADDR

XOR ax, ax
MOV ss, ax
MOV ds, ax
MOV sp, SP_TOP
MOV bx, os_hello_message
CALL display_message

reset_drive 0x00

; Load FAT section
load_fat:
    MOV bx, FAT_BASE_MEM_ADDR 
    XOR ax, ax
    MOV es, ax
    MOV cx, SECTORS_PER_FAT
    MOV ax, 1 
    CALL read_sector
    CMP cx, 0
    JE load_root_dir
    DEC cx
    ADD bx, 0x200   ; Advanced the address in BX by 512 bytes
    jmp load_fat

load_root_dir:
; Load Root section
    MOV bx, ROOT_DIR_BASE_MEM_ADDR
    XOR ax, ax
    MOV cx, SECTORS_FOR_ROOT_DIR
    MOV ax, 1 + (2 * SECTORS_PER_FAT) 
    CALL read_sector
    CMP cx, 0
    JE load_kernel_file 
    DEC cx
    ADD bx, 0x200
    jmp load_root_dir

load_kernel_file:
    XOR ax, ax
    MOV ds, ax
    MOV es, ax
    MOV dx, 224
    MOV ax, ROOT_DIR_BASE_MEM_ADDR
.read_one_entry:
    MOV si, ax
    MOV di, kernel_filename
    MOV cx, 11
    REPE CMPSB
    CMP cx, 0
    JE get_kernel_start_cluster_id
    ADD ax, 32
    DEC dx
    CMP dx, 0
    JA .read_one_entry
    MOV BX, error_not_kernel_message
    CALL display_message
    jmp final

get_kernel_start_cluster_id:
    ; Don't display, bug in display_message to overwrite the register
    ; MOV bx, kernel_file_found_message
    ; CALL display_message
    MOV bp, ax
    MOV bx, KERNEL_FILE_BASE_MEM_ADDR
    MOV dx, WORD[bp+26] ; read start cluster id
    MOV bp, FAT_BASE_MEM_ADDR

.load_kernel_to_mem_loop:
    MOV ax, dx                              ; Copy start cluster id from DX to AX
    ADD ax, 1 + 9 * 2 + (224 * 32) / 512 - 2   ; Calculate the real sector
    CALL read_sector                        ; Read sector
    MOV ax, dx                              ; Copy start cluster id from DX to AX
    MOV cx, 12                              ; 12bits per FAT entry
    MUL cx                                  ; 12bits per FAT entry 
    XOR dx, dx                              ; Clear DX for division
    MOV cx, 8                              ; 8 bit per byte
    DIV cx  ; AX: start bytes offset in FAT, DX: if not aligned, then the lower 4 bits
    mov si, ax
    MOV ax, word [bp + si]
    MOV cl, dl
    SHR ax, cl
    AND ax, 0x0fff
    CMP ax, 0x0fef
    JA init_gdt 
    MOV dx, ax
    ADD bx, 0x200
    JMP .load_kernel_to_mem_loop

init_gdt:
    MOV bx, kernel_file_load_complete
    CALL display_message

switch_vga_mode:
    ; Switch to 320x200x8 mode
    MOV al, 0x13
    MOV ah, 0x00
    INT 0x10
    MOV bx, switch_vga_mode_message
    CALL display_message

    ; Now the FAT table is not needed anymore
    ; Initialize the GDT
    MOV ax, 0
    MOV es, ax
    MOV di, 0x800

    ; GDT #0, NULL descriptor 
    MOV cx, 4
    REP STOSW

    ; GDT #1, code segment descriptor
    MOV es:[di], WORD 0xffff
    MOV es:[di+2], WORD 0x0000
    MOV es:[di+4], BYTE 0x00
    MOV es:[di+5], BYTE 0x9a
    MOV es:[di+6], BYTE 0xcf 
    MOV es:[di+7], BYTE 0x00
    add di, 8

    ; GDT #2, data segment descriptor
    MOV es:[di], WORD 0xffff
    MOV es:[di+2], WORD 0x0000
    MOV es:[di+4], WORD 0x00
    MOV es:[di+5], WORD 0x92
    MOV es:[di+6], WORD 0xcf
    MOV es:[di+7], WORD 0x00

    LGDT [gdt]

init_idt:
    CLI     ; Disable interrupt as we will set an empty IDT so no code to handle the interrupt
    LIDT [idt]

enable_a20:
    ; Fast A20 Method first
     in al, 0x92
     or al, 2
     out 0x92, al

init_protect_mode:
    ; Enable the 32bit protection mode
    MOV eax, cr0
    or eax, 1
    MOV cr0, eax

clean_prefetch_queue:
    ; First need to clean up the prefetch queue to remove any queued 16bit commands
    jmp prepare_32bit_registers
    nop
    nop
    nop 
    nop

prepare_32bit_registers:
    mov ax, 0x10
    mov ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax
    MOV esp, 0x2ffff

boot_to_kernel:
    ; 32bit long jump in 16bit mode
    DB 0x66
    DB 0xEA
    DD KERNEL_FILE_BASE_MEM_ADDR
    DW 0x0008
    ; Goodbyte BL2, now the kernel journey start!

final:
	hlt
	jmp final

; Display message function
; Parameters:
;   - BX: The start index address of the message, the message shall be end with '\0'
; Return:
;   - No return value
display_message:
    pusha
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
    popa
    RET

; Read a sector to target memory address
; Parameters:
;   - AX: Sector LBA
;   - ES:BX The target address
; Return:
;   - No return value
read_sector:
  xor     cx, cx                      ; Set try count = 0
 
.read_a_sec:
  push dx
  push    ax                          ; Store logical block
  push    cx                          ; Store try number
  push    bx                          ; Store data buffer offset
 
  ; Calculate cylinder, head and sector:
  ; Cylinder = (LBA / SectorsPerTrack) / NumHeads
  ; Sector   = (LBA mod SectorsPerTrack) + 1
  ; Head     = (LBA / SectorsPerTrack) mod NumHeads
 
  mov     bx, SECTORS_PER_TRACK       ; Get sectors per track
  xor     dx, dx
  div     bx                          ; Divide (dx:ax/bx to ax,dx)
                                      ; Quotient (ax) =  LBA / SectorsPerTrack
                                      ; Remainder (dx) = LBA mod SectorsPerTrack
  inc     dx                          ; Add 1 to remainder, since sector
  mov     cl, dl                      ; Store result in cl for int 13h call.
 
  mov     bx, NUM_OF_HEAD             ; Get number of heads
  xor     dx, dx
  div     bx                          ; Divide (dx:ax/bx to ax,dx)
                                      ; Quotient (ax) = Cylinder
                                      ; Remainder (dx) = head
  mov     ch, al                      ; ch = cylinder                      
  xchg    dl, dh                      ; dh = head number
 
  ; Call interrupt 0x13, subfunction 2 to actually
  ; read the sector.
  ; al = number of sectors
  ; ah = subfunction 2
  ; cx = sector number
  ; dh = head number
  ; dl = drive number
  ; es:bx = data buffer
  ; If it fails, the carry flag will be set.

  mov     ax, 0x0201                  ; Subfunction 2, read 1 sector
  mov     dl, BOOT_DRIVE              ; from this drive
  pop     bx                          ; Restore data buffer offset.
  int     0x13
  jc      .read_fail
 
  ; On success, return to caller.
  pop     cx                          ; Discard try number
  pop     ax                          ; Get logical block from stack
  pop dx
  ret
 
  ; The read has failed.
  ; We will retry four times total, then jump to boot failure.
.read_fail:   
  pop     cx                          ; Get try number             
  inc     cx                          ; Next try
  cmp     cx, 3              ; Stop at 3 tries
  je      .boot_failure
 
  ; Reset the disk system:
  xor     ax, ax
  int     0x13
 
  ; Get logical block from stack and retry.
  pop     ax
  jmp     .read_a_sec    

.boot_failure:
    MOV bx, error_message
    CALL display_message
    jmp final

BITS_PER_BYTE DB 8
BITS_PER_FAT_ENTRY DB 12

gdt DW 24
    DD 0x800

idt DW 2048
    DD 0x00

error_message DB "Failed to read disk", 0x0d, 0x0a, 0x00
error_not_kernel_message DB "Cannot find kernel file", 0x0d, 0x0a, 0x00
os_hello_message	DB "Booting Netium OS...", 0x0d, 0x0a, 0x00
kernel_file_found_message DB "Find kernel file, start to load it...", 0x0d, 0x0a, 0x00
kernel_file_load_complete DB "Complete to load the kernel...", 0x0d, 0x0a, 0x00
inited_gdt_message DB "GDT initialized!", 0x0d, 0x0a, 0x00
inited_idt_message DB "IDT initialized!", 0x0d, 0x0a, 0x00
enabled_a20_message DB "A20 enabled!", 0x0d, 0x0a, 0x00
switch_vga_mode_message DB "Switch VGA mode!", 0x0d, 0x0a, 0x00
kernel_filename    DB "KERNEL  SYS"

