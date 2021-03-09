#ifndef _GUI_H_
#define _GUI_H_

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

typedef struct {
    int res_width;
    int res_height;
    unsigned char *pvram;
    unsigned char *pcharfonts;
    int font_width;
    int font_height;
} screen_info_t;

void init_screen();

void render_ui();
void drawstring(screen_info_t *pscreen, int x, int y, char *str);
void drawchar(screen_info_t *pscreen, int x, int y, char ch, int palatte_id);
void boxfill8(unsigned char *vram, int xszie, unsigned char c, int x0, int y0, int x1, int y1);
void drawuint32(int x, int y, unsigned int n);
void drawpointer(int x, int y, void *p);

void boxfill8_ds(int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void writechar_dirty(char c);

#endif