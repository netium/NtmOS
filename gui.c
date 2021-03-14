// Author: Bo Zhou (zb.public@outlook.com)
// Under GPL 2.0 License

#include "gui.h"
#include "k_vga.h"
#include "font8x8_basic.h"
#include "kernel_inits.h"
#include "kstring.h"
#include "k_heap.h"

static screen_info_t g_screen_info;

static int cx, cy;

static int mouse_x, mouse_y;

layer_t * bg_window; 
layer_t * mouse_layer;

void initial_layers(); 
layer_t * alloc_layer();
void set_layer_vbuf(layer_t *p_layer, unsigned char *vbuf, int width, int height, int col_inv);
void mov_layer_z(layer_t *p_layer, int z);
void mov_layer(layer_t * layer, int vx0, int vy0);
void free_layer(layer_t * layer);
void refresh_screen();

void drawfont8_scr(int x, int y, char c, unsigned char ch);
void draw_uint_hex_scr(int x, int y, char c, unsigned int n);

void init_screen() {
    g_screen_info.width = 320;
    g_screen_info.height = 200;
    g_screen_info.pvram = (unsigned char *)0xa0000;
    g_screen_info.pcharfonts = (unsigned char *)font8x8_basic;
    g_screen_info.font_width = 8;
    g_screen_info.font_height = 8;

    cx = 0;
    cy = 0;

    init_palette();

    initial_layers();

    bg_window = alloc_layer();
    mouse_layer = alloc_layer();

    unsigned char * bg_vbuf = k_malloc(g_screen_info.width * g_screen_info.height);
    set_layer_vbuf(bg_window, bg_vbuf, g_screen_info.width, g_screen_info.height, -1);
    unsigned char * mouse_vbuf = k_malloc(8 * 8);
    set_layer_vbuf(mouse_layer, mouse_vbuf, 8, 8, 99);
    mov_layer(bg_window, 0, 0);
    mov_layer(mouse_layer, mouse_x, mouse_y);
    mov_layer_z(bg_window, 0);
    mov_layer_z(mouse_layer, 1);
}

void initial_layers() {
    g_screen_info.z_top = -1;
    for (int i = 0; i < LAYER_STACK_SIZE; i++) g_screen_info.layers->flags = 0;
}

layer_t * alloc_layer() {
    layer_t *layer = 0;
    for (int i = 0; i < LAYER_STACK_SIZE; i++) {
        if (0 == g_screen_info.layers[i].flags) {
            layer = &g_screen_info.layers[i];
            layer->flags |= 1;
            layer->z = -1;
            return layer;
        } 
    }

    return 0;
}

void mov_layer_z(layer_t *p_layer, int nz) {
    if (0 == p_layer) return;

    int z = 0;
    int old = p_layer->z;

    if (nz > g_screen_info.z_top + 1) nz = g_screen_info.z_top + 1;
    if (nz < -1) nz = -1;

    p_layer->z = nz;

    if (old > nz) {
        if (nz >= 0) {
            for (z = old; z > nz; z--) {
                g_screen_info.layer_stack[z] = g_screen_info.layer_stack[z - 1];
                g_screen_info.layer_stack[z]->z = z;
            }
            g_screen_info.layer_stack[nz] = p_layer;
        }
        else {
            if (g_screen_info.z_top > old) {
                for (z = old; z < g_screen_info.z_top; z++) {
                    g_screen_info.layer_stack[z] = g_screen_info.layer_stack[z + 1];
                    g_screen_info.layer_stack[z]->z = z;
                }
            }
            g_screen_info.z_top--;
        }
    }    
    else if (old < nz) {
       if (old >= 0) {
           for (z = old; z < nz; z++) {
               g_screen_info.layer_stack[z] = g_screen_info.layer_stack[z + 1];
               g_screen_info.layer_stack[z]->z = z;
           }
           g_screen_info.layer_stack[nz] = p_layer;
       } 
       else {
           for (z = g_screen_info.z_top; z >= nz; z--) {
               g_screen_info.layer_stack[z + 1] = g_screen_info.layer_stack[z];
               g_screen_info.layer_stack[z + 1] = z + 1;
           }
           g_screen_info.layer_stack[nz] = p_layer;
           g_screen_info.z_top++;
       }
    }

    refresh_screen();
}

void refresh_screen() {
    int bx, by, vx, vy;
    unsigned char *vbuf, c, *vram = g_screen_info.pvram;
    for (int z = 0; z <= g_screen_info.z_top; z++) {

        // boxfill8(g_screen_info.pvram, g_screen_info.width, COL8_00FFFF, 64, 64,  64 + 4, 64 + 4);

        layer_t *layer = g_screen_info.layer_stack[z];
        unsigned char *vbuf = layer->vbuf;
        for (by = 0; by < layer->height; by++) {
            vy = layer->y + by;
            for (bx = 0; bx < layer->width; bx++) {
                vx = layer->x + bx;
                c = vbuf[by * layer->width + bx];
                if (c != layer->col_inv) {
                    vram[vy * g_screen_info.width + vx] = c;
                }
            }
        }
    }
}
void mov_layer(layer_t * layer, int vx0, int vy0) {
    if (0 == layer) return;
    layer->x = vx0;
    layer->y = vy0;
    if (layer->z >= 0) refresh_screen();
}

void free_layer(layer_t * layer) {
    if (0 == layer) return;

    if (layer->z >= 0)
        mov_layer_z(layer, -1);

    layer->flags = 0;
}

void set_layer_vbuf(layer_t *p_layer, unsigned char *vbuf, int width, int height, int col_inv) {
    p_layer->vbuf = vbuf;
    p_layer->width = width;
    p_layer->height = height;
    p_layer->col_inv = col_inv;
}

void drawfont8(layer_t *pscreen, int x, int y, char c, unsigned char *font);

void render_ui(layer_t * bg_window) {
    if (0 == bg_window || 0 == bg_window->vbuf) return;

    boxfill8(bg_window->vbuf, bg_window->width, COL8_008484, 0, 0, bg_window->width - 1, bg_window->height - 29);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_C6C6C6, 0, bg_window->height - 28, bg_window->width - 1, bg_window->height - 28);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_FFFFFF, 0, bg_window->height - 27, bg_window->width - 1, bg_window->height - 27);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_C6C6C6, 0, bg_window->height - 26, bg_window->width - 1, bg_window->height - 1);

    boxfill8(bg_window->vbuf, bg_window->width, COL8_FFFFFF, 3, bg_window->height - 24, 59, bg_window->height - 24);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_FFFFFF, 2, bg_window->height - 24, 2, bg_window->height - 4);

    boxfill8(bg_window->vbuf, bg_window->width, COL8_848484, 3, bg_window->height - 4, 59, bg_window->height - 4);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_848484, 59, bg_window->height - 23, 59, bg_window->height - 5);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_000000, 2, bg_window->height - 3, 59, bg_window->height - 3);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_000000, 60, bg_window->height-24, 60, bg_window->height - 3);

    boxfill8(bg_window->vbuf, bg_window->width, COL8_848484, bg_window->width - 47, bg_window->height - 24, bg_window->width - 4, bg_window->height - 24);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_848484, bg_window->width-47, bg_window->height-23, bg_window->width-47, bg_window->height - 4);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_FFFFFF, bg_window->width-47, bg_window->height - 3, bg_window->width-4, bg_window->height -3);
    boxfill8(bg_window->vbuf, bg_window->width, COL8_FFFFFF, bg_window->width -3 , bg_window->height - 24, bg_window->width - 3, bg_window->height -3 );

    drawstring(bg_window, 0, 0, "NtmOS v0.0.0.1");

    refresh_screen();
}

void drawstring(layer_t *pscreen, int x, int y, char *str) {
    if (pscreen == 0) return;

    int ch;

    while ((ch = *str++) != 0) {
        drawfont8(pscreen, x, y, COL8_FFFFFF, font8x8_basic[ch]);
        x += g_screen_info.font_width;
    }
}

void drawchar(layer_t *pscreen, int x, int y, char ch, int palatte_id) {
    if (pscreen == 0)
        return;

    if (x < 0 || x >= pscreen->width - pscreen->width)
        return;

    if (y < 0 || y >= pscreen->height - g_screen_info.font_height)
        return;

    int char_start_index = ((g_screen_info.font_width *g_screen_info.font_height) >> 3) * ch;

    unsigned char *pfont =g_screen_info.pcharfonts + char_start_index;

    unsigned char *pvram_addr;

    for (int i = 0; i < 8; i++) {
        pvram_addr = pscreen->vbuf + (y + i) * pscreen->width + x;
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

void drawpointer(layer_t * pscreen, int x, int y, void *p) {
    drawuint32(pscreen, x, y, (unsigned int)p);
}

void drawuint32(layer_t * pscreen, int x, int y, unsigned int n) {
    if (0 == pscreen || 0 == pscreen->vbuf) return ;

    if (n == 0) drawfont8(pscreen, x, y, COL8_FFFFFF, font8x8_basic['0']);

    while (n != 0) {
        int digit = n % 10;
        n = n / 10;
        drawfont8 (pscreen, x, y, COL8_FFFFFF, font8x8_basic['0' + digit]);
        x += 8;
    }
}
void drawfont8(layer_t *pscreen, int x, int y, char c, unsigned char *font) {
    if (0 == pscreen || 0 == pscreen->vbuf) return;

    int i;
    unsigned char *p;
    char d;
    for (i = 0; i < 8; i++) {
        p = pscreen->vbuf + (y + i) * pscreen->width + x;
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

void move_mouse(int dx, int dy) {
    drawuint32(bg_window, 56, 56, 8);

    refresh_screen();

    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x > g_screen_info.width) mouse_x = g_screen_info.width;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y > g_screen_info.height) mouse_y = g_screen_info.height;

    boxfill8(g_screen_info.pvram, g_screen_info.width, COL8_0000FF, mouse_x, mouse_y, mouse_x + 4, mouse_y + 4);
}

void draw_uint_hex_scr(int x, int y, char c, unsigned int n) {
    unsigned int mask = 0x0000000F;
    unsigned int shift = 28;
    for (int i = 0; i < 8; i++) {
        int x = (n >> shift) & mask;
        if (x >= 0 && x <= 9) {
            drawfont8_scr(x, y, c, '0' + x);
        }
        else {
            drawfont8_scr(x, y, c, (x - 10) + 'A');
        }
        x + g_screen_info.font_width;
        shift -= 4;
    }
}

void drawfont8_scr(int x, int y, char c, unsigned char ch) {
    if (g_screen_info.pvram == 0) return;

    unsigned char *font = font8x8_basic[ch];

    int i;
    unsigned char *p;
    char d;
    for (i = 0; i < 8; i++) {
        p = g_screen_info.pvram + (y + i) * g_screen_info.width + x;
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