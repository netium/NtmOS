; This file is contains all macro that callable in 16bit real mode

; MACRO reset_boot_drive
; Function: To reset the specific drive status
; Parameters:
;   %1: the drive index
%macro reset_drive 1
    MOV ah, 0x00
    MOV dl, %1
    INT 0x13
%endmacro

%macro infinite_hlt 0
.__hlt_loop
    hlt
    JMP __hlt_loop
%endmacro

%macro read_drive_sectors 6
    MOV ah, 0x02
    MOV al, %1
    MOV ch, %2
    MOV cl, %3
    MOV dh, %4
    MOV dl, %5
    MOV bx, %6
    INT 0x13
%endmacro