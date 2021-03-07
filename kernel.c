// Author: Bo Zhou (zb.public@outlook.com)
// Under GPLv2.0 lincense

#include "kernel_functions.h"
#include "gui.h"

void kernel_main(void) {

	render_ui();

	while (1) _io_hlt();

	return;
}
