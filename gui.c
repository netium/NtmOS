// Author: Bo Zhou (zb.public@outlook.com)
// Under GPL 2.0 License

#include "gui.h"
#include "k_vga.h"
#include "font8x8_basic.h"
#include "kernel_inits.h"

static screen_info_t g_screen_info;

static int cx, cy;

void init_screen() {
    g_screen_info.res_width = 320;
    g_screen_info.res_height = 200;
    g_screen_info.pvram = (unsigned char *)0xa0000;
    g_screen_info.pcharfonts = (unsigned char *)font8x8_basic;
    g_screen_info.font_width = 8;
    g_screen_info.font_height = 8;

    cx = 0;
    cy = 0;

    init_palette();
}

void drawfont8(screen_info_t *pscreen, int x, int y, char c, unsigned char *font);

void render_ui() {
    if (0 == g_screen_info.pvram) return;

    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_008484, 0, 0, g_screen_info.res_width - 1, g_screen_info.res_height - 29);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_C6C6C6, 0, g_screen_info.res_height - 28, g_screen_info.res_width - 1, g_screen_info.res_height - 28);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_FFFFFF, 0, g_screen_info.res_height - 27, g_screen_info.res_width - 1, g_screen_info.res_height - 27);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_C6C6C6, 0, g_screen_info.res_height - 26, g_screen_info.res_width - 1, g_screen_info.res_height - 1);

    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_FFFFFF, 3, g_screen_info.res_height - 24, 59, g_screen_info.res_height - 24);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_FFFFFF, 2, g_screen_info.res_height - 24, 2, g_screen_info.res_height - 4);

    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_848484, 3, g_screen_info.res_height - 4, 59, g_screen_info.res_height - 4);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_848484, 59, g_screen_info.res_height - 23, 59, g_screen_info.res_height - 5);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_000000, 2, g_screen_info.res_height - 3, 59, g_screen_info.res_height - 3);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_000000, 60, g_screen_info.res_height-24, 60, g_screen_info.res_height - 3);

    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_848484, g_screen_info.res_width - 47, g_screen_info.res_height - 24, g_screen_info.res_width - 4, g_screen_info.res_height - 24);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_848484, g_screen_info.res_width-47, g_screen_info.res_height-23, g_screen_info.res_width-47, g_screen_info.res_height - 4);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_FFFFFF, g_screen_info.res_width-47, g_screen_info.res_height - 3, g_screen_info.res_width-4, g_screen_info.res_height -3);
    boxfill8(g_screen_info.pvram, g_screen_info.res_width, COL8_FFFFFF, g_screen_info.res_width -3 , g_screen_info.res_height - 24, g_screen_info.res_width - 3, g_screen_info.res_height -3 );

    drawstring(&g_screen_info, 0, 0, "NtmOS v0.0.0.1");
}

void drawstring(screen_info_t *pscreen, int x, int y, char *str) {
    if (pscreen == 0) return;

    int ch;

    while ((ch = *str++) != 0) {
        drawfont8(pscreen, x, y, COL8_FFFFFF, font8x8_basic[ch]);
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

void drawpointer(int x, int y, void *p) {
    drawuint32(x, y, (unsigned int)p);
}

void drawuint32(int x, int y, unsigned int n) {
    screen_info_t *pscreen = &g_screen_info;

    if (n == 0) drawfont8(pscreen, x, y, COL8_FFFFFF, font8x8_basic['0']);

    while (n != 0) {
        int digit = n % 10;
        n = n / 10;
        drawfont8 (pscreen, x, y, COL8_FFFFFF, font8x8_basic['0' + digit]);
        x += 8;
    }
}
void drawfont8(screen_info_t *pscreen, int x, int y, char c, unsigned char *font) {
    int i;
    unsigned char *p;
    char d;
    for (i = 0; i < 8; i++) {
        p = pscreen->pvram + (y + i) * pscreen->res_width + x;
        d = font[i];

        if ((d & 0x80) != 0) p[7] = c; else p[7] = COL8_000000;
        if ((d & 0x40) != 0) p[6] = c; else p[6] = COL8_000000;
        if ((d & 0x20) != 0) p[5] = c; else p[5] = COL8_000000;
        if ((d & 0x10) != 0) p[4] = c; else p[4] = COL8_000000;
        if ((d & 0x08) != 0) p[3] = c; else p[3] = COL8_000000;
        if ((d & 0x04) != 0) p[2] = c; else p[2] = COL8_000000;
        if ((d & 0x02) != 0) p[1] = c; else p[1] = COL8_000000;
        if ((d & 0x01) != 0) p[0] = c; else p[0] = COL8_000000;
    } 
}

void boxfill8_ds(int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
    unsigned char *vram = g_screen_info.pvram;

    if (0 == vram) return;

    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
}

void writechar_dirty(char c) {
    screen_info_t *pscreen = &g_screen_info;
}