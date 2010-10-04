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
#include <stdbool.h>
#include <stdint.h>
#include "hardware.h"
#include "timer.h"
#include "font.h"
#include "led.h"
#include "bits.h"
#include "sign.h"

/* timer for testing */
static struct itimer Timer;

/* text that is displayed on sign */
static char Sign_Text[16];

/* sign dimensions */
#define SIGN_X_MAX 16
#define SIGN_Y_MAX 5
/* bitmap of text on sign */
static bool Sign_Bitmap[SIGN_X_MAX][SIGN_Y_MAX];

static bool sign_bitmap(uint8_t x, uint8_t y)
{
    bool bit_status = false;

    if ((x < SIGN_X_MAX) && (y < SIGN_Y_MAX)) {
        bit_status = Sign_Bitmap[x][y];
    }

    return bit_status;
}

static void sign_bitmap_set(uint8_t x, uint8_t y, bool state)
{
    if ((x < SIGN_X_MAX) && (y < SIGN_Y_MAX)) {
        Sign_Bitmap[x][y] = state;
    }
}

void sign_character_set(uint8_t x_in, char ch)
{
    uint8_t x = 0, y = 0;
    uint8_t width = 0;
    uint8_t bitmap = 0;
    uint8_t mask = 0;
    bool status = 0;

    if (x_in < SIGN_X_MAX) {
        for (y = 0; y < SIGN_Y_MAX; y++) {
            bitmap = font_bitmap(ch, y);
            width = font_width(ch);
            mask = 0x80;
            for (x = x_in; x < SIGN_X_MAX; x++) {
                if (bitmap & mask) {
                    status = true;
                } else {
                    status = false;
                }
                sign_bitmap_set(x, y, status);
                width--;
                if (width == 0) {
                    break;
                }
                mask = mask>>1;
            }
        }
    }
}

void sign_clear(void)
{
    uint8_t x, y;

    for (x = 0; x < SIGN_X_MAX; x++) {
        for (y = 0; y < SIGN_Y_MAX; y++) {
            Sign_Bitmap[x][y] = false;
        }
    }
}

static void sign_test1(void)
{
    static char ch = ' ';

    sign_clear();
    sign_character_set(0, ch);
    if (ch == 127) {
        ch = ' ';
    } else {
        ch++;
    }
}

static void sign_test2(void)
{
    static uint8_t x = 0;
    uint8_t width = 0;

    sign_clear();
    sign_character_set(x, 127);
    if (x < SIGN_X_MAX) {
        x++;
    } else {
        x = 0;
    }
}

void sign_task(void)
{
    static uint8_t last_y = 0;
    static uint8_t last_x = 0;
    static uint8_t y = 0;
    static uint8_t x = 0;

    /* handle the communication timer */
    led_off(last_y, last_x);
    /* adjust next x, y */
    x++;
    if (x >= SIGN_X_MAX) {
        x = 0;
        y++;
        if (y >= SIGN_Y_MAX) {
            y = 0;
        }
    }
    /* set the pixel if lit */
    if (sign_bitmap(x, y)) {
        led_on(y, x);
    } else {
        led_off(y, x);
    }
    last_y = y;
    last_x = x;

    if (timer_interval_expired(&Timer)) {
        timer_interval_reset(&Timer);
        sign_test2();
    }
}

void sign_init(void)
{
    timer_interval_start(&Timer, 500);
    snprintf(Sign_Text, sizeof(Sign_Text), "JOSHUA");
}
