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

#define MAX_BITMAPS 6
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
};

uint8_t font_width(char ch)
{
    uint8_t width = 0;
    uint8_t offset = 0;

    if ((ch >= ' ') && (ch <= '"')) {
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

    if ((ch >= ' ') && (ch <= '"')) {
        offset = ch - ' ';
        if ((offset < MAX_BITMAPS) &&
            (y < MAX_BIT_Y)) {
            bitmap = font_8x5_bitmaps[offset].map[y];
        }
    }

    return bitmap;
}
