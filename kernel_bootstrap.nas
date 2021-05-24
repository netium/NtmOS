; This is the kernel bootstrap file
; As the kernel_main is written in C, so some of the bootstrap functions are not easy to express in the kernel_main so this bootstrap
; is where any boostrap operation need to be done before going into the full function kernel_main

GLOBAL kernel_bootstrap_main

EXTERN kernel_main

KERN_BASE_VIR_ADDR EQU 0x80000000
KERN_BASE_PHY_ADDR EQU 0x08000000
STACK_TMP_PHY_ADDR  EQU 0x300000
STACK_VIR_ADDR  EQU (KERN_BASE_VIR_ADDR + 0x300000)
CODE_PHY_ADDR EQU (KERN_BASE_PHY_ADDR + 0x100000)
CODE_VIR_ADDR EQU (KERN_BASE_VIR_ADDR + 0x100000)
CODE_TMP_PHY_ADDR EQU 0xa000
CODE_SIZE EQU 0x96000
MEM_PAGE_SIZE EQU 0x1000

PAGE_DIR_ADDR EQU (KERN_BASE_PHY_ADDR + 0x1000)
PAGE_DIR_KERN_LINEAR_START_INDEX EQU 0x200
PAGE_TAB_1_ADDR EQU (KERN_BASE_PHY_ADDR + 0x2000)
PAGE_TAB_200_ADDR EQU (KERN_BASE_PHY_ADDR + 0x3000)

section .text

; We are here, so we shall happy, because now we are in protect mode with more than 1MB to be addressable
; The only pity thing is that as it's currently in 32bit protect mode it cannot use 16bit BIOS interrupt anymore.
kernel_bootstrap_main:
    ; If eip < 1M, then this means that the kernel is still in < 1MB RAM so we need to move it
    mov esp, STACK_TMP_PHY_ADDR
call .get_eip
.get_eip:
    pop eax
    cmp eax, CODE_VIR_ADDR
    jae .jump_to_kernal_main
.move_kernel:    ; move kernel to 1M 
    cld          ; Setup the move direction
    mov ecx, CODE_SIZE / 4      
    mov eax, CODE_TMP_PHY_ADDR
    mov esi, eax
    mov eax, CODE_PHY_ADDR
    mov edi, eax
    rep movsd
; Start to initialize the paging, but this is the very simple one, just to make sure that the kernel
; can run from the 2GB virtual address, and then kernel run into the kernel_main a full paging mechanism
; will be built up
.init_paging_directory:
    ; First, mark all 1024 page directory entry to 0x00000000
    cld
    xor eax, eax
    mov edi, PAGE_DIR_ADDR
    mov ecx, 1024
    rep stosd
    ; Setup the page_dir_entry[0], which refer to the first [0MB, 4MB) 
    mov esi, PAGE_DIR_ADDR
    mov dword [esi], (PAGE_TAB_1_ADDR & 0xFFFFF000) | 0x1
    ; Setup the page_dir_entry[200H], which map the [2GB, 2GB+4MB] to [0MB, 4MB)
    mov ecx, PAGE_DIR_KERN_LINEAR_START_INDEX
    mov dword [esi + ecx * 4], (PAGE_TAB_200_ADDR & 0xFFFFF000) | 0x01
.init_paging_1_tables:
    mov esi, PAGE_TAB_1_ADDR
    xor ecx, ecx
    mov ebx, 0x1
.init_paging_1_tables_loop:
    cmp ecx, 1024
    jae .init_paging_200_tables
    mov dword [esi + ecx * 4], ebx;
    ; add esi, 4
    add ebx, MEM_PAGE_SIZE
    inc ecx
    jmp .init_paging_1_tables_loop
.init_paging_200_tables:
    mov esi, PAGE_TAB_200_ADDR
    xor ecx, ecx
    mov ebx, KERN_BASE_PHY_ADDR + 0x1
.init_paging_200_tables_loop:
    cmp ecx, 1024
    jae .enable_paging
    mov dword [esi + ecx * 4], ebx;
    add ebx, MEM_PAGE_SIZE
    inc ecx
    jmp .init_paging_200_tables_loop
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
.jump_new_kernel_addr   ; After kernel moving and paging enable, then need to jump to the virtual address of the kernel
    mov esp, STACK_VIR_ADDR     ; Reinitialize the stack point to virtual memory address
    mov eax, CODE_VIR_ADDR
    push eax
    ret
.jump_to_kernal_main:
    MOV esp, STACK_VIR_ADDR    ; Initial the stack pointer
    SUB esp, 4
    JMP kernel_main