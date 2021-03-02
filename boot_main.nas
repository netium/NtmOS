MISMATCH: "        .file   "boot_main.c""
        .code16gcc: 
        .text: 
MISMATCH: "#APP"
        .code16gcc: 
MISMATCH: "#NO_APP"
MISMATCH: "        .globl  boot_main"
MISMATCH: "        .type   boot_main, @function"
boot_main: 
boot_main.LFB0: 
        boot_main.cfi_startproc: 
MISMATCH: "        pushl   %ebp"
MISMATCH: "        .cfi_def_cfa_offset 8"
MISMATCH: "        .cfi_offset 5, -8"
MISMATCH: "        movl    %esp, %ebp"
MISMATCH: "        .cfi_def_cfa_register 5"
MISMATCH: "        pushl   %ebx"
MISMATCH: "        subl    $4, %esp"
MISMATCH: "        .cfi_offset 3, -12"
        call    __x86.get_pc_thunk.ax
MISMATCH: "        addl    $_GLOBAL_OFFSET_TABLE_, %eax"
MISMATCH: "        movl    %eax, %ebx"
        call    _io_hlt@PLT
        nop
MISMATCH: "        addl    $4, %esp"
MISMATCH: "        popl    %ebx"
MISMATCH: "        .cfi_restore 3"
MISMATCH: "        popl    %ebp"
MISMATCH: "        .cfi_restore 5"
MISMATCH: "        .cfi_def_cfa 4, 4"
        ret
        boot_main.cfi_endproc: 
boot_main.LFE0: 
MISMATCH: "        .size   boot_main, .-boot_main"
        boot_main.section: .text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat: 
MISMATCH: "        .globl  __x86.get_pc_thunk.ax"
MISMATCH: "        .hidden __x86.get_pc_thunk.ax"
MISMATCH: "        .type   __x86.get_pc_thunk.ax, @function"
__x86.get_pc_thunk.ax: 
__x86.get_pc_thunk.ax.LFB1: 
        __x86.get_pc_thunk.ax.cfi_startproc: 
MISMATCH: "        movl    (%esp), %eax"
        ret
        __x86.get_pc_thunk.ax.cfi_endproc: 
__x86.get_pc_thunk.ax.LFE1: 
MISMATCH: "        .ident  "GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0""
        __x86.get_pc_thunk.ax.section: .note.GNU-stack,"",@progbits: 

