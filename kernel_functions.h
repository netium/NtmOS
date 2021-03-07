#ifndef _KERNEL_FUNCTIONS_H_

#define _KERNEL_FUNCTIONS_H_

void _io_hlt(void);
void _io_cli(void);

void _io_out8(int port, int data);
void _io_out16(int port, int data);
void _io_out32(int port, int data);

int _io_in8(int port);
int _io_in16(int port);
int _io_in32(int port);

int _get_eflags();
void _set_eflags(int eflags);

int _get_eip();

#endif

