#ifndef _GUI_H_
#define _GUI_H_

#include "kstring.h"

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

#define LAYER_STACK_SIZE 256

typedef struct {
    int x, y;
    int width, height;
} rectangle_t;

typedef struct {
    unsigned char *vbuf;
    int x, y;
    int width, height;
    int z;
    int col_inv, flags;
} layer_t;

typedef struct {
    int width;
    int height;
    unsigned char *pvram;
    unsigned char *pcharfonts;
    int font_width;
    int font_height;
    int z_top;

    layer_t *layer_stack[LAYER_STACK_SIZE];
    layer_t layers[LAYER_STACK_SIZE];

} screen_info_t;

void init_screen();

void initial_interrupt_event_queue();

void render_ui(layer_t * bg_window);
void drawstring(layer_t *pscreen, int x, int y, char *str);
void drawchar(layer_t *pscreen, int x, int y, char ch, int palatte_id);
void boxfill8(unsigned char *vram, int xszie, unsigned char c, int x0, int y0, int x1, int y1);
void drawuint32(layer_t *pscreen, int x, int y, unsigned int n);
void drawpointer(layer_t *pscreen, int x, int y, void *p);

void boxfill8_ds(int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void writechar_dirty(char c);

void move_mouse(int dx, int dy);
void draw_uint_hex_scr(int x, int y, char c, unsigned int n);

void k_printf(const char *str);
void tui_move_cursor(unsigned int x, unsigned int y);

typedef enum {
    OK,
    WARNING,
    ERROR
} info_level_t;

void tui_get_cursor_position(unsigned int *px, unsigned int *py);
void tui_update_cursor(unsigned int x, unsigned int y);

void tui_putchar(char ch);

#endif