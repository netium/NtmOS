global int20h_handler_stub
global int21h_handler_stub
global int24h_handler_stub
global int27h_handler_stub
global int2ch_handler_stub

extern int20h_handler
extern int21h_handler
extern int24h_handler
extern int27h_handler
extern int2ch_handler

.text

int20h_handler_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	cld
	mov ebx, (2 << 3)
	mov ds, ebx
	mov es, ebx
	mov fs, ebx
	mov gs, ebx
	call int20h_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

int21h_handler_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	cld
	mov ebx, (2 << 3)
	mov ds, ebx
	mov es, ebx
	mov fs, ebx
	mov gs, ebx
	call int21h_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

int24h_handler_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	cld
	mov ebx, (2 << 3)
	mov ds, ebx
	mov es, ebx
	mov fs, ebx
	mov gs, ebx
	call int24h_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

int27h_handler_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	cld
	mov ebx, (2 << 3)
	mov ds, ebx
	mov es, ebx
	mov fs, ebx
	mov gs, ebx
	call int27h_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

int2ch_handler_stub:
	pushad
	push ds
	push es
	push fs
	push gs
	cld
	mov ebx, (2 << 3)
	mov ds, ebx
	mov es, ebx
	mov fs, ebx
	mov gs, ebx
	call int2ch_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

