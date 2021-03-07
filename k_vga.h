#ifndef _K_VGA_H

#define _K_VGA_H

#include "kernel_functions.h"

void init_palette(void);
void set_palette(int start, int end, unsigned char *pRGB);

#endif