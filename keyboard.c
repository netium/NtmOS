#include "keyboard.h"

#define VT_NUL (0x00)
#define VT_BEL (0x07)
#define VT_BS  (0x08)
#define VT_TAB (0x09)
#define VT_LF  (0x0a)
#define VT_VT  (0x0b)
#define VT_FF  (0x0c)
#define VT_CR  (0x0d)
#define VT_ESC (0x1b)
#define VT_DEL (0x7f)

const keycode_t g_keyboard_scancode_set_1[] = {
    /* 0x0000 */ {VT_NUL, KEY_DOWN},    /* No used */
    /* 0x0001 */ {VT_ESC, KEY_DOWN},         /* escape pressed */
    /* 0x0002 */ {'1', KEY_DOWN},       /* 1 pressed */
    /* 0x0003 */ {'2', KEY_DOWN},       /* 2 pressed */
    /* 0x0004 */ {'3', KEY_DOWN},       /* 3 pressed */
    /* 0x0005 */ {'4', KEY_DOWN},       /* 4 pressed */
    /* 0x0006 */ {'5', KEY_DOWN},       /* 5 pressed */
    /* 0x0007 */ {'6', KEY_DOWN},       /* 6 pressed */
    /* 0x0008 */ {'7', KEY_DOWN},       /* 7 pressed */
    /* 0x0009 */ {'8', KEY_DOWN},       /* 8 pressed */
    /* 0x000A */ {'9', KEY_DOWN},       /* 9 pressed */
    /* 0x000B */ {'0', KEY_DOWN},       /* 0 pressed */
    /* 0x000C */ {'-', KEY_DOWN},       /* - pressed */
    /* 0x000D */ {'=', KEY_DOWN},       /* = presssed */
    /* 0x000E */ {VT_BS, KEY_DOWN},      /* backspace pressed */
    /* 0x000F */ {VT_TAB, KEY_DOWN},    /* TAB pressed */

    /* 0x0010 */ {'Q', KEY_DOWN},       /* Q pressed */
    /* 0x0011 */ {'W', KEY_DOWN},       /* W pressed */
    /* 0x0012 */ {'E', KEY_DOWN},       /* E pressed */
    /* 0x0013 */ {'R', KEY_DOWN},       /* R pressed */
    /* 0x0014 */ {'T', KEY_DOWN},       /* T pressed */
    /* 0x0015 */ {'Y', KEY_DOWN},       /* Y pressed */
    /* 0x0016 */ {'U', KEY_DOWN},       /* U pressed */
    /* 0x0017 */ {'I', KEY_DOWN},       /* I pressed */
    /* 0x0018 */ {'O', KEY_DOWN},       /* O pressed */
    /* 0x0019 */ {'P', KEY_DOWN},       /* P pressed */
    /* 0x001A */ {'[', KEY_DOWN},       /* [ pressed */
    /* 0x001B */ {']', KEY_DOWN},       /* ] pressed */
    /* 0x001C */ {0, KEY_DOWN},         /* enter pressed */
    /* 0x001D */ {0, KEY_DOWN},       /* left ctrl pressed */
    /* 0x001E */ {'A', KEY_DOWN},       /* A pressed */
    /* 0x001F */ {'S', KEY_DOWN},       /* S pressed */

    /* 0x0020 */ {'D', KEY_DOWN},         /* D pressed */
    /* 0x0021 */ {'F', KEY_DOWN},         /* F pressed */
    /* 0x0022 */ {'G', KEY_DOWN},         /* G pressed */
    /* 0x0023 */ {'H', KEY_DOWN},         /* H pressed */
    /* 0x0024 */ {'J', KEY_DOWN},         /* J pressed */
    /* 0x0025 */ {'K', KEY_DOWN},         /* K pressed */
    /* 0x0026 */ {'L', KEY_DOWN},         /* L pressed */
    /* 0x0027 */ {';', KEY_DOWN},         /* ; pressed */
    /* 0x0028 */ {'\'', KEY_DOWN},         /* ' pressed */
    /* 0x0029 */ {'`', KEY_DOWN},         /* ` pressed */
    /* 0x002A */ {0, KEY_DOWN},            /* left shift pressed */
    /* 0x002B */ {'\\', KEY_DOWN},         /* \ pressed */
    /* 0x002C */ {'Z', KEY_DOWN},         /* Z pressed */
    /* 0x002D */ {'X', KEY_DOWN},         /* X pressed */
    /* 0x002E */ {'C', KEY_DOWN},         /* C pressed */
    /* 0x002F */ {'V', KEY_DOWN},         /* V pressed */

    /* 0x0030 */ {'B', KEY_DOWN},         /* B pressed */
    /* 0x0031 */ {'N', KEY_DOWN},         /* N pressed */
    /* 0x0032 */ {'M', KEY_DOWN},         /* M pressed */
    /* 0x0033 */ {',', KEY_DOWN},         /* , pressed */
    /* 0x0034 */ {'.', KEY_DOWN},         /* . pressed */
    /* 0x0035 */ {'/', KEY_DOWN},         /* / pressed */
    /* 0x0036 */ {0, KEY_DOWN},         /* right shift pressed */
    /* 0x0037 */ {'*', KEY_DOWN},         /* (keypad)* pressed */
    /* 0x0038 */ {0, KEY_DOWN},         /* left alt pressed */
    /* 0x0039 */ {' ', KEY_DOWN},         /* space pressed */
    /* 0x003A */ {0, KEY_DOWN},            /* CapsLock pressed */
    /* 0x003B */ {0, KEY_DOWN},         /* F1 pressed */
    /* 0x003C */ {0, KEY_DOWN},         /* F2 pressed */
    /* 0x003D */ {0, KEY_DOWN},         /* F3 pressed */
    /* 0x003E */ {0, KEY_DOWN},         /* F4 pressed */
    /* 0x003F */ {0, KEY_DOWN},         /* F5 pressed */

    /* 0x0040 */ {0, KEY_DOWN},         /* F6 pressed */
    /* 0x0041 */ {0, KEY_DOWN},         /* F7 pressed */
    /* 0x0042 */ {0, KEY_DOWN},         /* F8 pressed */
    /* 0x0043 */ {0, KEY_DOWN},         /* F9 pressed */
    /* 0x0044 */ {0, KEY_DOWN},         /* F10 pressed */
    /* 0x0045 */ {0, KEY_DOWN},         /* NumLock pressed */
    /* 0x0046 */ {0, KEY_DOWN},         /* ScrollLock pressed */
    /* 0x0047 */ {'7', KEY_DOWN},         /* (keypad)7 pressed */
    /* 0x0048 */ {'8', KEY_DOWN},         /* (keypad)8 pressed */
    /* 0x0049 */ {'9', KEY_DOWN},         /* (keypad)9 pressed */
    /* 0x004A */ {'-', KEY_DOWN},            /* (keypad)- pressed */
    /* 0x004B */ {'4', KEY_DOWN},         /* (keypad)4 pressed */
    /* 0x004C */ {'5', KEY_DOWN},         /* (keypad)5 pressed */
    /* 0x004D */ {'6', KEY_DOWN},         /* (keypad)6 pressed */
    /* 0x004E */ {'+', KEY_DOWN},         /* (keypad)+ pressed */
    /* 0x004F */ {'1', KEY_DOWN},         /* (keypad)1 pressed */

    /* 0x0050 */ {'2', KEY_DOWN},         /* (keypad)2 pressed */
    /* 0x0051 */ {'3', KEY_DOWN},         /* (keypad)3 pressed */
    /* 0x0052 */ {'0', KEY_DOWN},         /* (keypad)0 pressed */
    /* 0x0053 */ {'.', KEY_DOWN},         /* (keypad). pressed */
    /* 0x0054 */ {0, KEY_DOWN},         /*  No used */
    /* 0x0055 */ {0, KEY_DOWN},         /* No used */
    /* 0x0056 */ {0, KEY_DOWN},         /* No used */
    /* 0x0057 */ {0, KEY_DOWN},         /* No used */
    /* 0x0058 */ {0, KEY_DOWN},         /* No used */
    /* 0x0059 */ {0, KEY_DOWN},         /* No used */
    /* 0x005A */ {0, KEY_DOWN},            /* No used */
    /* 0x005B */ {0, KEY_DOWN},         /* No used */
    /* 0x005C */ {0, KEY_DOWN},         /* No used */
    /* 0x005D */ {0, KEY_DOWN},         /* No used */
    /* 0x005E */ {0, KEY_DOWN},         /* No used */
    /* 0x005F */ {0, KEY_DOWN},         /* No used */

    /* 0x0060 */ {0, KEY_DOWN},         /* No used */
    /* 0x0061 */ {0, KEY_DOWN},         /* No used */
    /* 0x0062 */ {0, KEY_DOWN},         /* No used */
    /* 0x0063 */ {0, KEY_DOWN},         /* No used */
    /* 0x0064 */ {0, KEY_DOWN},         /*  No used */
    /* 0x0065 */ {0, KEY_DOWN},         /* No used */
    /* 0x0066 */ {0, KEY_DOWN},         /* No used */
    /* 0x0067 */ {0, KEY_DOWN},         /* No used */
    /* 0x0068 */ {0, KEY_DOWN},         /* No used */
    /* 0x0069 */ {0, KEY_DOWN},         /* No used */
    /* 0x006A */ {0, KEY_DOWN},            /* No used */
    /* 0x006B */ {0, KEY_DOWN},         /* No used */
    /* 0x006C */ {0, KEY_DOWN},         /* No used */
    /* 0x006D */ {0, KEY_DOWN},         /* No used */
    /* 0x006E */ {0, KEY_DOWN},         /* No used */
    /* 0x006F */ {0, KEY_DOWN},         /* No used */

    /* 0x0070 */ {0, KEY_DOWN},         /* No used */
    /* 0x0071 */ {0, KEY_DOWN},         /* No used */
    /* 0x0072 */ {0, KEY_DOWN},         /* No used */
    /* 0x0073 */ {0, KEY_DOWN},         /* No used */
    /* 0x0074 */ {0, KEY_DOWN},         /*  No used */
    /* 0x0075 */ {0, KEY_DOWN},         /* No used */
    /* 0x0076 */ {0, KEY_DOWN},         /* No used */
    /* 0x0077 */ {0, KEY_DOWN},         /* No used */
    /* 0x0078 */ {0, KEY_DOWN},         /* No used */
    /* 0x0079 */ {0, KEY_DOWN},         /* No used */
    /* 0x007A */ {0, KEY_DOWN},            /* No used */
    /* 0x007B */ {0, KEY_DOWN},         /* No used */
    /* 0x007C */ {0, KEY_DOWN},         /* No used */
    /* 0x007D */ {0, KEY_DOWN},         /* No used */
    /* 0x007E */ {0, KEY_DOWN},         /* No used */
    /* 0x007F */ {0, KEY_DOWN},         /* No used */

    /* 0x0080 */ {VT_NUL, KEY_UP},    /* No used */
    /* 0x0081 */ {VT_ESC, KEY_UP},         /* escape up */
    /* 0x0082 */ {'1', KEY_UP},       /* 1 up */
    /* 0x0083 */ {'2', KEY_UP},       /* 2 up */
    /* 0x0084 */ {'3', KEY_UP},       /* 3 up */
    /* 0x0085 */ {'4', KEY_UP},       /* 4 up */
    /* 0x0086 */ {'5', KEY_UP},       /* 5 up */
    /* 0x0087 */ {'6', KEY_UP},       /* 6 up */
    /* 0x0088 */ {'7', KEY_UP},       /* 7 up */
    /* 0x0089 */ {'8', KEY_UP},       /* 8 up */
    /* 0x008A */ {'9', KEY_UP},       /* 9 up */
    /* 0x008B */ {'0', KEY_UP},       /* 0 up */
    /* 0x008C */ {'-', KEY_UP},       /* - up */
    /* 0x008D */ {'=', KEY_UP},       /* = presssed */
    /* 0x008E */ {VT_BS, KEY_UP},      /* backspace up */
    /* 0x008F */ {VT_TAB, KEY_UP},    /* TAB up */

    /* 0x0090 */ {'Q', KEY_UP},       /* Q up */
    /* 0x0091 */ {'W', KEY_UP},       /* W up */
    /* 0x0092 */ {'E', KEY_UP},       /* E up */
    /* 0x0093 */ {'R', KEY_UP},       /* R up */
    /* 0x0094 */ {'T', KEY_UP},       /* T up */
    /* 0x0095 */ {'Y', KEY_UP},       /* Y up */
    /* 0x0096 */ {'U', KEY_UP},       /* U up */
    /* 0x0097 */ {'I', KEY_UP},       /* I up */
    /* 0x0098 */ {'O', KEY_UP},       /* O up */
    /* 0x0099 */ {'P', KEY_UP},       /* P up */
    /* 0x009A */ {'[', KEY_UP},       /* [ up */
    /* 0x009B */ {']', KEY_UP},       /* ] up */
    /* 0x009C */ {0, KEY_UP},         /* enter up */
    /* 0x009D */ {0, KEY_UP},       /* left ctrl up */
    /* 0x009E */ {'A', KEY_UP},       /* A up */
    /* 0x009F */ {'S', KEY_UP},       /* S up */

    /* 0x00A0 */ {'D', KEY_UP},         /* D up */
    /* 0x00A1 */ {'F', KEY_UP},         /* F up */
    /* 0x00A2 */ {'G', KEY_UP},         /* G up */
    /* 0x00A3 */ {'H', KEY_UP},         /* H up */
    /* 0x00A4 */ {'J', KEY_UP},         /* J up */
    /* 0x00A5 */ {'K', KEY_UP},         /* K up */
    /* 0x00A6 */ {'L', KEY_UP},         /* L up */
    /* 0x00A7 */ {';', KEY_UP},         /* ; up */
    /* 0x00A8 */ {'\'', KEY_UP},         /* ' up */
    /* 0x00A9 */ {'`', KEY_UP},         /* ` up */
    /* 0x00AA */ {0, KEY_UP},            /* left shift up */
    /* 0x00AB */ {'\\', KEY_UP},         /* \ up */
    /* 0x00AC */ {'Z', KEY_UP},         /* Z up */
    /* 0x00AD */ {'X', KEY_UP},         /* X up */
    /* 0x00AE */ {'C', KEY_UP},         /* C up */
    /* 0x00AF */ {'V', KEY_UP},         /* V up */

    /* 0x00B0 */ {'B', KEY_UP},         /* B up */
    /* 0x00B1 */ {'N', KEY_UP},         /* N up */
    /* 0x00B2 */ {'M', KEY_UP},         /* M up */
    /* 0x00B3 */ {',', KEY_UP},         /* , up */
    /* 0x00B4 */ {'.', KEY_UP},         /* . up */
    /* 0x00B5 */ {'/', KEY_UP},         /* / up */
    /* 0x00B6 */ {0, KEY_UP},         /* right shift up */
    /* 0x00B7 */ {'*', KEY_UP},         /* (keypad)* up */
    /* 0x00B8 */ {0, KEY_UP},         /* left alt up */
    /* 0x00B9 */ {' ', KEY_UP},         /* space up */
    /* 0x00BA */ {0, KEY_UP},            /* CapsLock up */
    /* 0x00BB */ {0, KEY_UP},         /* F1 up */
    /* 0x00BC */ {0, KEY_UP},         /* F2 up */
    /* 0x00BD */ {0, KEY_UP},         /* F3 up */
    /* 0x00BE */ {0, KEY_UP},         /* F4 up */
    /* 0x00BF */ {0, KEY_UP},         /* F5 up */

    /* 0x00C0 */ {0, KEY_UP},         /* F6 up */
    /* 0x00C1 */ {0, KEY_UP},         /* F7 up */
    /* 0x00C2 */ {0, KEY_UP},         /* F8 up */
    /* 0x00C3 */ {0, KEY_UP},         /* F9 up */
    /* 0x00C4 */ {0, KEY_UP},         /* F10 up */
    /* 0x00C5 */ {0, KEY_UP},         /* NumLock up */
    /* 0x00C6 */ {0, KEY_UP},         /* ScrollLock up */
    /* 0x00C7 */ {'7', KEY_UP},         /* (keypad)7 up */
    /* 0x00C8 */ {'8', KEY_UP},         /* (keypad)8 up */
    /* 0x00C9 */ {'9', KEY_UP},         /* (keypad)9 up */
    /* 0x00CA */ {'-', KEY_UP},            /* (keypad)- up */
    /* 0x00CB */ {'4', KEY_UP},         /* (keypad)4 up */
    /* 0x00CC */ {'5', KEY_UP},         /* (keypad)5 up */
    /* 0x00CD */ {'6', KEY_UP},         /* (keypad)6 up */
    /* 0x00CE */ {'+', KEY_UP},         /* (keypad)+ up */
    /* 0x00CF */ {'1', KEY_UP},         /* (keypad)1 up */

    /* 0x00D0 */ {'2', KEY_UP},         /* (keypad)2 up */
    /* 0x00D1 */ {'3', KEY_UP},         /* (keypad)3 up */
    /* 0x00D2 */ {'0', KEY_UP},         /* (keypad)0 up */
    /* 0x00D3 */ {'.', KEY_UP},         /* (keypad). up */
    /* 0x00D4 */ {0, KEY_UP},         /*  No used */
    /* 0x00D5 */ {0, KEY_UP},         /* No used */
    /* 0x00D6 */ {0, KEY_UP},         /* No used */
    /* 0x00D7 */ {0, KEY_UP},         /* No used */
    /* 0x00D8 */ {0, KEY_UP},         /* No used */
};

const int g_keyboard_scancode_set_1_size = sizeof(g_keyboard_scancode_set_1) / sizeof(g_keyboard_scancode_set_1[0]);

