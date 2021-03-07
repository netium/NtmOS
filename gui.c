#include "gui.h"
#include "k_vga.h"

void render_ui() {
    screen_info_t screen_info;
    screen_info.res_width = 320;
    screen_info.res_height = 200;
    screen_info.pvram = (unsigned char *)0xa0000;
    screen_info.pcharfonts = (unsigned char *)0;
    screen_info.font_width = 8;
    screen_info.font_height = 16;

    int xsize, ysize;

    init_palette();

    xsize = 320;
    ysize = 200;

    boxfill8(screen_info.pvram, xsize, COL8_008484, 0, 0, xsize - 1, ysize - 29);
    boxfill8(screen_info.pvram, xsize, COL8_C6C6C6, 0, ysize - 28, xsize - 1, ysize - 28);
    boxfill8(screen_info.pvram, xsize, COL8_FFFFFF, 0, ysize - 27, xsize - 1, ysize - 27);
    boxfill8(screen_info.pvram, xsize, COL8_C6C6C6, 0, ysize - 26, xsize - 1, ysize - 1);

    boxfill8(screen_info.pvram, xsize, COL8_FFFFFF, 3, ysize - 24, 59, ysize - 24);
    boxfill8(screen_info.pvram, xsize, COL8_FFFFFF, 2, ysize - 24, 2, ysize - 4);

    boxfill8(screen_info.pvram, xsize, COL8_848484, 3, ysize - 4, 59, ysize - 4);
    boxfill8(screen_info.pvram, xsize, COL8_848484, 59, ysize - 23, 59, ysize - 5);
    boxfill8(screen_info.pvram, xsize, COL8_000000, 2, ysize - 3, 59, ysize - 3);
    boxfill8(screen_info.pvram, xsize, COL8_000000, 60, ysize-24, 60, ysize - 3);

    boxfill8(screen_info.pvram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize - 4, ysize - 24);
    boxfill8(screen_info.pvram, xsize, COL8_848484, xsize-47, ysize-23, xsize-47, ysize - 4);
    boxfill8(screen_info.pvram, xsize, COL8_FFFFFF, xsize-47, ysize - 3, xsize-4, ysize -3);
    boxfill8(screen_info.pvram, xsize, COL8_FFFFFF, xsize -3 , ysize - 24, xsize - 3, ysize -3 );

    // drawstring(&screen_info, 0, 0, "Hello, world!");
}

void drawstring(screen_info_t *pscreen, int line, int col, unsigned char *str) {
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

    for (int i = 0; i < 16; i++) {
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