// asm(".code16gcc");

#include "kernel_functions.h"
#include "gui.h"

void kernel_main(void) {

	render_ui();

	while (1) _io_hlt();

	return;
}
