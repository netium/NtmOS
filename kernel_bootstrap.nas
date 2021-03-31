; This is the kernel bootstrap file
; As the kernel_main is written in C, so some of the bootstrap functions are not easy to express in the kernel_main so this bootstrap
; is where any boostrap operation need to be done before going into the full function kernel_main

GLOBAL kernel_boostrap_main

EXTERN kernel_main

section .text

kernel_boostrap_main:
    MOV esp, 0x2ffff    ; Initial the stack pointer
    JMP kernel_main