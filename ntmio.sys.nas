; Ntmio.sys
; The first file for the Netium OS

%include "macro16.nas"

SP_TOP EQU 0x7c00	; To define the address of top of the stack
START_MEM_ADDR EQU 0x0800	; The start address of the code in memory
NUM_OF_HEAD EQU 2
SECTORS_PER_TRACK EQU 18
KERNEL_FILE_BASE_MEM_ADDR EQU 0x8000

ORG START_MEM_ADDR

MOV ax, 0x00
MOV ss, ax
MOV ds, ax
MOV sp, SP_TOP
MOV bx, os_hello_message
CALL display_message

reset_drive 0x00

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

os_hello_message	DB "Booting Netium OS...", 0x0d, 0x0a, 0x00
