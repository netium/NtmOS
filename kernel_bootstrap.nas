; This is the kernel bootstrap file
; As the kernel_main is written in C, so some of the bootstrap functions are not easy to express in the kernel_main so this bootstrap
; is where any boostrap operation need to be done before going into the full function kernel_main

GLOBAL kernel_bootstrap_main

EXTERN kernel_main

STACK_PHY_ADDR  EQU 0x300000
CODE_PHY_ADDR EQU 0x100000 
CODE_TMP_PHY_ADDR EQU 0xa000
CODE_SIZE EQU 0x96000

PAGE_DIR_ADDR EQU 0xa000
PAGE_TAB_1_ADDR EQU 0xb000

section .text

kernel_bootstrap_main:
    ; If eip < 1M, then this means that the kernel is still in < 1MB RAM so we need to move it
    mov esp, STACK_PHY_ADDR
call .get_eip
.get_eip:
    pop eax
    cmp eax, CODE_PHY_ADDR
    jae .init 
.move_kernel:    ; move kernel to 1M and then jump to there 
    cld          ; Setup the move direction
    mov ecx, CODE_SIZE      
    mov eax, CODE_TMP_PHY_ADDR
    mov esi, eax
    mov eax, CODE_PHY_ADDR
    mov edi, eax
    rep movsb
.jump_new_kernel_addr   ; After kernel moving, then need to jump to a new address of the kernel
    mov eax, CODE_PHY_ADDR
    push eax
    ret
.init:
.init_paging_directory:
    mov esi, PAGE_DIR_ADDR
    xor ecx, ecx
    mov dword [esi], (PAGE_TAB_1_ADDR & 0xFFFFF000) | 0x1
    inc ecx
.init_paging_tables:
    mov esi, PAGE_TAB_1_ADDR
    xor ecx, ecx
    mov ebx, 0x1
.init_paging_tables_loop:
    cmp ecx, 1024
    jae .enable_paging
    mov dword [esi + ecx * 4], ebx;
    ; add esi, 4
    add ebx, 0x1000
    inc ecx
    jmp .init_paging_tables_loop
    ; mov dword[eax], 
;.fill_page_table:
;    mov ecx, ebx
;    or ecx, 3
;    mov [table_768+eax*4], ecx
;    add ebx, 4096
;    inc eax
;    cmp eax, 1024
;    je .end
;    jmp .fill_page_table
;.end:
.enable_paging:
    mov eax, PAGE_DIR_ADDR
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
.jump_to_kernal_main:
    MOV esp, STACK_PHY_ADDR    ; Initial the stack pointer
    SUB esp, 4
    JMP kernel_main