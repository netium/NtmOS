; Ntmio.sys
; The first file for the Netium OS

%include "macro16.nas"

SP_TOP EQU 0x7c00	; To define the address of top of the stack
START_MEM_ADDR EQU 0x07E00	; The start address of the code in memory
NUM_OF_HEAD EQU 2
SECTORS_PER_TRACK EQU 18
SECTORS_PER_FAT EQU 9
SECTORS_FOR_ROOT_DIR EQU (224 * 32) / 512
BOOT_DRIVE EQU 0x00

KERNEL_FILE_BASE_MEM_ADDR EQU 0x8000

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
    MOV bx, WORD[bp+26] ; read start cluster id
    MOV cx, WORD[bp+28] ; read size

final:
	hlt
	jmp final

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

; Read a sector to target memory address
; Parameters:
;   - AX: Sector LBA
;   - ES:BX The target address
; Return:
;   - No return value
read_sector:
  xor     cx, cx                      ; Set try count = 0
 
.read_a_sec:
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

error_message DB "Failed to read disk", 0x0d, 0x0a, 0x00
error_not_kernel_message DB "Cannot find kernel file", 0x0d, 0x0a, 0x00
os_hello_message	DB "Booting Netium OS...", 0x0d, 0x0a, 0x00
kernel_file_found_message DB "Find kernel file, start to load it...", 0x0d, 0x0a, 0x00
kernel_filename    DB "KERNEL  SYS"
