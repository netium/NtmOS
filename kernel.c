// asm(".code16gcc");

void kernel_main(void) {
//	_io_hlt();
	char *p;
	p = (char*)0xa0000;
	for (int i = 0; i < 0xc0000; i++) {
		p[i] = i % 255;
	}

	while(1);

	return;
}
