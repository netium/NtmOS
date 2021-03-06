; ntmOS
; TAB=4

CYLS EQU 10
SP_BOTTOM EQU 0x7BFF
MBR_MEM_ADDR EQU 0x7c00
BOOT_DRIVE EQU 0x00
BL2_BASE_MEM EQU 0x07E00

SECTORS_PER_TRACK EQU 18
NUM_OF_HEAD EQU 2

%include "macro16.nas"

    ORG MBR_MEM_ADDR

    JMP start   ; Sector offset:0x000, short jump with (0xEB 0x?? 0x90)
    DB 0x90
    DB "NETIUMOS"   ; Sector offset:0x003, OEM Name, 8bytes

    ; DOS 4.0 EBPB, refer to: https://en.wikipedia.org/wiki/BIOS_parameter_block
    DW 512  ; Sector offset:0x00B, Bytes per logical sector
    DB 1    ; Sector offset:0x00D, Logical sectors per cluster
    DW 1    ; Sector offset:0x00E, Reserved logical sectors
    DB 2    ; Sector offset:0x010, Number of FATs
    DW 224  ; Sector offset:0x011, Root directory entries
    DW 2880 ; Sector offset:0x013, Total logical sectors
    DB 0xf0 ; Sector offset:0x015, Media descriptor
    DW 9    ; Sector offset:0x016, Logical sectors per FAT
    DW SECTORS_PER_TRACK ; Sector offset:0x018, Physical sectors per track
    DW 2    ; Sector offset:0x01A, Number of heads
    DD 0    ; Sector offset:0x01C, Hidden sectors
    DD 0    ; Sector offset:0x020, Large total logical sectors
    DB 0    ; Sector offset:0x024, Physical drive number
    DB 0    ; Sector offset:0x025, Flags etc.
    DB 0x29 ; Sector offset:0x026, Extended boot signature, 0x29 means that the following three fields are available
    DD 0xFFFFFFFF       ; Sector offset:0x27,   Volume serial number
    DB "NO NAME    "    ; Sector offset:0x02B, Volume label
    DB "FAT16   "       ; Sector offset:0x36,   File-system type
    ; DB 18 DUP (0)

start:
    MOV ax, 0
    MOV ss, ax
    MOV sp, MBR_MEM_ADDR
    MOV ds, ax
    MOV bx, bootloader_greeting_message
    CALL display_message
    reset_drive BOOT_DRIVE
;   Shall check whether the reset is successful here

; Load disk section
load_boot_module:
    MOV bx, BL2_BASE_MEM
    XOR ax, ax
    MOV es, ax
    MOV ax, 1 + 2 * 9 + (224 * 32 / 512)
.load_loop:    
    CALL read_sector
    CMP ax, 1 + 2 * 9 + (224 * 32 / 512) + 5
    JA bl2_load_complete
    INC ax
    ADD bx, 0x200   ; Advanced the address in BX by 512 bytes
    jmp .load_loop

bl2_load_complete:
    MOV bx, bl2_load_complete_message
    CALL display_message
    ; MOV [0x0ff0], ch    ; Copy the # of boot sectors to 0x0ff0
    JMP BL2_BASE_MEM      ; Load complete, jump to the loaded code, never come back

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

; Display message function
; Parameters:
;   - BX: The start index address of the message, the message shall be end with '\0'
; Return:
;   - No return value
display_message:
    MOV si, bx
.display_loop:
    MOV al, [si]
    ADD si, 1
    CMP al, 0
    JE .ret
    MOV ah, 0x0e
    MOV bx, 15
    INT 0x10
    JMP .display_loop
.ret:
    RET

disk_load_error:
    MOV bx, error_message
    CALL display_message
    JMP final

final:
    hlt
    jmp final

i_am_here_message    DB "H",0x0

error_message    DB "E", 0x0

read_message    DB "R", 0x0

bl2_load_complete_message DB "BL2 load complete", 0x0d, 0x0a, 0x00
bootloader_greeting_message    DB "NtM-Bootloader 0.1", 0x0d, 0x0a, 0x00

    DB 0x1fe-($-$$) DUP (0)

    DB 0x55, 0xaa   ; Sector offset:0x1FE, Boot sector signature (0x55, 0xAA)

    ; FAT table 1
    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 4600 DUP (0)

    ; FAT table 2
    DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    DB 1469432 DUP (0)
