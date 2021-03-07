#include "gui.h"
#include "k_vga.h"
#include "font8x8_basic.h"

void render_ui() {
    screen_info_t screen_info;
    screen_info.res_width = 320;
    screen_info.res_height = 200;
    screen_info.pvram = (unsigned char *)0xa0000;
    screen_info.pcharfonts = (unsigned char *)font8x8_basic;
    screen_info.font_width = 8;
    screen_info.font_height = 8;

    init_palette();

    boxfill8(screen_info.pvram, screen_info.res_width, COL8_008484, 0, 0, screen_info.res_width - 1, screen_info.res_height - 29);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_C6C6C6, 0, screen_info.res_height - 28, screen_info.res_width - 1, screen_info.res_height - 28);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_FFFFFF, 0, screen_info.res_height - 27, screen_info.res_width - 1, screen_info.res_height - 27);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_C6C6C6, 0, screen_info.res_height - 26, screen_info.res_width - 1, screen_info.res_height - 1);

    boxfill8(screen_info.pvram, screen_info.res_width, COL8_FFFFFF, 3, screen_info.res_height - 24, 59, screen_info.res_height - 24);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_FFFFFF, 2, screen_info.res_height - 24, 2, screen_info.res_height - 4);

    boxfill8(screen_info.pvram, screen_info.res_width, COL8_848484, 3, screen_info.res_height - 4, 59, screen_info.res_height - 4);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_848484, 59, screen_info.res_height - 23, 59, screen_info.res_height - 5);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_000000, 2, screen_info.res_height - 3, 59, screen_info.res_height - 3);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_000000, 60, screen_info.res_height-24, 60, screen_info.res_height - 3);

    boxfill8(screen_info.pvram, screen_info.res_width, COL8_848484, screen_info.res_width - 47, screen_info.res_height - 24, screen_info.res_width - 4, screen_info.res_height - 24);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_848484, screen_info.res_width-47, screen_info.res_height-23, screen_info.res_width-47, screen_info.res_height - 4);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_FFFFFF, screen_info.res_width-47, screen_info.res_height - 3, screen_info.res_width-4, screen_info.res_height -3);
    boxfill8(screen_info.pvram, screen_info.res_width, COL8_FFFFFF, screen_info.res_width -3 , screen_info.res_height - 24, screen_info.res_width - 3, screen_info.res_height -3 );

    drawstring(&screen_info, 1, 1, "Hello, world!");
}

void drawstring(screen_info_t *pscreen, int line, int col, char *str) {
    if (pscreen == 0)
        return;

    int nline = pscreen->res_height / pscreen->font_height;
    int ncol = pscreen->res_width / pscreen->font_width;

    if (line < 0 || line >= nline) return;
    if (col < 0 || col >= ncol) return;

    int x = col * pscreen->font_width;
    int y = line * pscreen->font_height;

    int ch;
    while ((ch = *str++) != 0) {
        drawchar(pscreen, x, y, ch, COL8_FFFFFF);
        x += pscreen->font_width;
    }
}

void drawchar(screen_info_t *pscreen, int x, int y, char ch, int palatte_id) {
    if (pscreen == 0)
        return;

    if (x < 0 || x >= pscreen->res_width - pscreen->font_width)
        return;

    if (y < 0 || y >= pscreen->res_height - pscreen->font_height)
        return;

    int char_start_index = ((pscreen->font_width * pscreen->font_height) >> 3) * ch;

    unsigned char *pfont = pscreen->pcharfonts + char_start_index;

    unsigned char *pvram_addr;

    for (int i = 0; i < 8; i++) {
        pvram_addr = pscreen->pvram + (y + i) * pscreen->res_width + x;
        int d = pfont[i];
        if ((d & 0x80) != 0) pvram_addr[0] = palatte_id;
        else if ((d & 0x40) != 0) pvram_addr[1] = palatte_id;
        else if ((d & 0x20) != 0) pvram_addr[2] = palatte_id;
        else if ((d & 0x10) != 0) pvram_addr[3] = palatte_id;
        else if ((d & 0x08) != 0) pvram_addr[4] = palatte_id;
        else if ((d & 0x04) != 0) pvram_addr[5] = palatte_id;
        else if ((d & 0x02) != 0) pvram_addr[6] = palatte_id;
        else if ((d & 0x01) != 0) pvram_addr[7] = palatte_id;
    }
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
}