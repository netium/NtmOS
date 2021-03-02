asm(".code16gcc");

void _io_hlt(void);

void boot_main(void) {
	// _io_hlt();
	int a = 1, b = 2;
	a = a + b;
}
