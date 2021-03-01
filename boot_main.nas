        ;FILE "boot_main.c"
        SECTION .text
        GLOBAL  boot_main
        GLOBAL boot_main:function
boot_main: 
.LFB0: 
MISMATCH: "        .cfi_startproc"
MISMATCH: "        endbr64"
MISMATCH: "        pushq   %rbp"
MISMATCH: "        .cfi_def_cfa_offset 16"
MISMATCH: "        .cfi_offset 6, -16"
        movq    rbp,rsp
MISMATCH: "        .cfi_def_cfa_register 6"
        call    _io_hlt@PLT
        nop
MISMATCH: "        popq    %rbp"
MISMATCH: "        .cfi_def_cfa 7, 8"
        ret
MISMATCH: "        .cfi_endproc"
.LFE0: 
        GLOBAL  boot_main:function (.-boot_main)
        ;IDENT "GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
MISMATCH: "        .section        .note.GNU-stack,"",@progbits"
MISMATCH: "        .section        .note.gnu.property,"a""
        ALIGN 8
MISMATCH: "        .long    1f - 0f"
MISMATCH: "        .long    4f - 1f"
        dd       5
.l0: 
        db       'GNU'
.l1: 
        ALIGN 8
        dd       0c0000002h
MISMATCH: "        .long    3f - 2f"
.l2: 
        dd       03h
.l3: 
        ALIGN 8
.l4: 

