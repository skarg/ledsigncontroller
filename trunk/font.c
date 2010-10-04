/*####COPYRIGHTBEGIN####
 ---------------------------------------------------------------
 Copyright (C) 2010 Steve Karg <skarg@users.sourceforge.net>

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ---------------------------------------------------------------
####COPYRIGHTEND####*/
#include "font.h"

#define MAX_BIT_X 8
#define MAX_BIT_Y 5
struct font_info {
    uint8_t width;
    uint8_t map[MAX_BIT_Y];
};

#define MAX_BITMAPS 96
/* Character bitmaps for Arial 8pt */
static struct font_info font_8x5_bitmaps[MAX_BITMAPS] =
{
    /* ' ' */
    {2, {0x00, 0x00, 0x00, 0x00, 0x00}},
    /* '!' */
    {1, {0x80, 0x80, 0x80, 0x00, 0x80}},
    /* '"' */
    {3, {0xA0, 0xA0, 0xA0, 0x00, 0x00}},
    /* '#' */
    {5, {0x28, 0xF8, 0x50, 0xF8, 0xA0}},
    /* '$' */
    {5, {0x70, 0xA8, 0x28, 0xA8, 0x70}},
    /* '%' */
    {8, {0x44, 0xA8, 0x54, 0x2A, 0x44}},
    /* '&' */
    {6, {0x30, 0x48, 0x30, 0x48, 0x74}},
    /*  ''' */
    {1, {0x80, 0x80, 0x00, 0x00, 0x00}},
    /*  '(' */
    {3, {0x20, 0x40, 0x80, 0x40, 0x20}},
    /*  ')' */
    {3, {0x80, 0x40, 0x20, 0x40, 0x80}},
    /*  '*' */
    {3, {0x40, 0xE0, 0x40, 0xA0, 0x00}},
    /*  '+' */
    {5, {0x20, 0x20, 0xF8, 0x20, 0x20}},
    /*  ',' */
    {1, {0x00, 0x00, 0x00, 0x80, 0x80}},
    /*  '-' */
    {3, {0x00, 0x00, 0xE0, 0x00, 0x00}},
    /*  '.' */
    {1, {0x00, 0x00, 0x00, 0x80, 0x00}},
    /*  '/'  */
    {3, {0x20, 0x40, 0x40, 0x40, 0x80}},
    /*  '0'  */
    {5, {0x70, 0x88, 0x88, 0x88, 0x70}},
    /*  '1'  */
    {3, {0x20, 0x60, 0x20, 0x20, 0x20}},
    /*  '2'  */
    {5, {0x70, 0x88, 0x10, 0x20, 0xF8}},
    /*  '3'  */
    {5, {0x70, 0x08, 0x38, 0x08, 0x70}},
    /*  '4'  */
    {4, {0x10, 0x30, 0x50, 0x70, 0x10}},
    /*  '5'  */
    {5, {0x70, 0x80, 0x60, 0x10, 0x70}},
    /*  '6'  */
    {5, {0x70, 0x88, 0x70, 0x88, 0x70}},
    /* '7'   */
    {5, {0x78, 0x08, 0x10, 0x20, 0x20}},
    /* '8'   */
    {5, {0x70, 0x88, 0xF8, 0x88, 0x70}},
    /* '9'   */
    {5, {0x70, 0x88, 0x78, 0x08, 0x70}},
    /* ':'   */
    {1, {0x00, 0x80, 0x00, 0x80, 0x00}},
    /*  ';'  */
    {1, {0x00, 0x80, 0x00, 0x80, 0x80}},
    /*  '<'  */
    {5, {0x08, 0x70, 0x80, 0x70, 0x08}},
    /*  '='   */
    {5, {0x00, 0xF8, 0x00, 0xF8, 0x00}},
    /*  '>'  */
    {5, {0x80, 0x70, 0x08, 0x70, 0x80}},
    /*  '?'  */
    {5, {0x70, 0x88, 0x30, 0x00, 0x20}},
    /*  '@'  */
    {4, {0xF8, 0xB8, 0xD8, 0xE8, 0xF8}},
    /* 'A' */
    {4, {0x20, 0x50, 0x70, 0x00, 0x00}},
    /* 'B' */
    {4, {0x60, 0x50, 0x60, 0x50, 0x60}},
    /* 'C' */
    {4, {0x30, 0x40, 0x40, 0x40, 0x30}},
    /* 'D' */
    {4, {0x60, 0x50, 0x50, 0x50, 0x60}},
    /* 'E' */
    {4, {0x70, 0x40, 0x70, 0x40, 0x70}},
    /* 'F' */
    {4, {0x70, 0x40, 0x70, 0x40, 0x40}},
    /*'G' */
    {4, {0x60, 0x80, 0xE0, 0xA0, 0x60}},
    /*'H' */
    {4, {0x50, 0x50, 0x70, 0x50, 0x50}},
    /*'I' */
    {4, {0x70, 0x20, 0x20, 0x20, 0x70}},
    /*'J' */
    {4, {0x70, 0x20, 0x20, 0xA0, 0x40}},
    /*'K' */
    {4, {0x05, 0x50, 0x60, 0x50, 0x50}},
    /*'L' */
    {4, {0x40, 0x40, 0x40, 0x40, 0x70}},
    /*'M' */
    {5, {0x88, 0xD8, 0xD8, 0xA8, 0xA8}},
    /*'N' */
    {5, {0xC8, 0xC8, 0xF8, 0x98, 0x98}},
    /*'O' */
    {4, {0x20, 0x50, 0x50, 0x50, 0x20}},
    /*'P' */
    {4, {0x60, 0x50, 0x60, 0x40, 0x40}},
    /*'Q' */
    {4, {0x40, 0xA0, 0xA0, 0xE0, 0x50}},
    /*'R' */
    {4, {0x60, 0x50, 0x60, 0x50, 0x50}},
    /*'S' */
    {4, {0x30, 0x40, 0x20, 0x10, 0x60}},
    /*'T' */
    {4, {0x70, 0x20, 0x20, 0x20, 0x20}},
    /*'U' */
    {4, {0x50, 0x50, 0x50, 0x50, 0x70}},
    /*'V' */
    {4, {0x50, 0x50, 0x50, 0x50, 0x20}},
    /*'W' (*/
    {4, {0xA8, 0xA8, 0xA8, 0xA8, 0x50}},
    /*'X' */
    {4, {0x50, 0x50, 0x20, 0x50, 0x50}},
    /*'Y' */
    {4, {0x50, 0x50, 0x50, 0x20, 0x20}},
    /*'Z' */
    {4, {0x70, 0x10, 0x20, 0x40, 0x70}},
    /*'[' */
    {3, {0x60, 0x40, 0x40, 0x40, 0x60}},
    /*'\' */
    {4, {0x40, 0x20, 0x20, 0x20, 0x10}},
    /*']' */
    {4, {0x60, 0x20, 0x20, 0x20, 0x60}},
    /*'^ OR DELICIOUS BLOCK' */
    {4, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},
    /*'_ OR EVERY OTHER ONE 1' */
    {4, {0xAA, 0x55, 0xAA, 0x55, 0xAA}},
    /*'` OR EVERY OTHER ONE 2' */
    {4, {0x55, 0xAA, 0x55, 0xAA, 0x55}},
    /* 'a' */
    {4, {0x20, 0x50, 0x70, 0x00, 0x00}},
    /* 'b' */
    {4, {0x60, 0x50, 0x60, 0x50, 0x60}},
    /* 'c' */
    {4, {0x30, 0x40, 0x40, 0x40, 0x30}},
    /* 'd' */
    {4, {0x60, 0x50, 0x50, 0x50, 0x60}},
    /* 'e' */
    {4, {0x70, 0x40, 0x70, 0x40, 0x70}},
    /* 'f' */
    {4, {0x70, 0x40, 0x70, 0x40, 0x40}},
    /*'g' */
    {4, {0x60, 0x80, 0xE0, 0xA0, 0x60}},
    /*'h' */
    {4, {0x50, 0x50, 0x70, 0x50, 0x50}},
    /*'i' */
    {4, {0x70, 0x20, 0x20, 0x20, 0x70}},
    /*'j' */
    {4, {0x70, 0x20, 0x20, 0xA0, 0x40}},
    /*'k' */
    {4, {0x05, 0x50, 0x60, 0x50, 0x50}},
    /*'l' */
    {4, {0x40, 0x40, 0x40, 0x40, 0x70}},
    /*'m' */
    {5, {0x88, 0xD8, 0xD8, 0xA8, 0xA8}},
    /*'n' */
    {5, {0xC8, 0xC8, 0xF8, 0x98, 0x98}},
    /*'o' */
    {4, {0x20, 0x50, 0x50, 0x50, 0x20}},
    /*'p' */
    {4, {0x60, 0x50, 0x60, 0x40, 0x40}},
    /*'q' */
    {4, {0x40, 0xA0, 0xA0, 0xE0, 0x50}},
    /*'r' */
    {4, {0x60, 0x50, 0x60, 0x50, 0x50}},
    /*'s' */
    {4, {0x30, 0x40, 0x20, 0x10, 0x60}},
    /*'t' */
    {4, {0x70, 0x20, 0x20, 0x20, 0x20}},
    /*'u' */
    {4, {0x50, 0x50, 0x50, 0x50, 0x70}},
    /*'v' */
    {4, {0x50, 0x50, 0x50, 0x50, 0x20}},
    /*'w' (*/
    {4, {0xA8, 0xA8, 0xA8, 0xA8, 0x50}},
    /*'x' */
    {4, {0x50, 0x50, 0x20, 0x50, 0x50}},
    /*'y' */
    {4, {0x50, 0x50, 0x50, 0x20, 0x20}},
    /*'z' */
    {4, {0x70, 0x10, 0x20, 0x40, 0x70}},
    /*'{' */
    {4, {0x20, 0x40, 0x60, 0x40, 0x20}},
    /*'|' */
    {4, {0x80, 0x80, 0x80, 0x80, 0x80}},
    /*'}' */
    {4, {0x40, 0x20, 0x60, 0x20, 0x40}},
    /*'~' */
    {4, {0xE0, 0x90, 0x09, 0x07, 0x00}},
    /* DEL */
    {8, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}
};

static bool font_character_valid(char ch)
{
    bool status = false;

    if ((ch >= ' ') && (ch <= '%')) {
        status = true;
    }

    return status;
}

uint8_t font_width(char ch)
{
    uint8_t width = 0;
    uint8_t offset = 0;

    if (font_character_valid(ch)) {
        offset = ch - ' ';
        if (offset < MAX_BITMAPS) {
            width = font_8x5_bitmaps[offset].width;
        }
    }

    return width;
}

uint8_t font_bitmap(char ch, uint8_t y)
{
    uint8_t bitmap = 0;
    uint8_t offset = 0;

    if (font_character_valid(ch)) {
        offset = ch - ' ';
        if ((offset < MAX_BITMAPS) &&
            (y < MAX_BIT_Y)) {
            bitmap = font_8x5_bitmaps[offset].map[y];
        }
    }

    return bitmap;
}
