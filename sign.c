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
#include <stdio.h>
#include <string.h>
#include "hardware.h"
#include "timer.h"
#include "font.h"
#include "led.h"
#include "bits.h"
#include "sign.h"
#include "timer0.h"

static SIGN_STATE Sign_State;
static SIGN_STATE Sign_Next_State;

/* timer for testing */
static struct itimer Timer;

/* character that is displayed on the sign */
static char Sign_Character = '-';

/* number of columns to space between letters */
#define SCROLL_GAP 2

/* sign dimensions */
#define SIGN_X_MAX 17
#define SIGN_Y_MAX 5
/* bitmap of text on sign */
static volatile bool Sign_Bitmap[SIGN_X_MAX][SIGN_Y_MAX];

/* text that is displayed on sign */
static char Sign_Text[128];
/* the x position of the name */
static int Name_Scroll_X = SIGN_X_MAX - 1;

void sign_state_set(SIGN_STATE state)
{
    if (state < SIGN_STATE_MAX) {
        Sign_State = state;
    }
}

void sign_character_set(char ch)
{
    Sign_Character = ch;
}

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

/* allow off sign characters using negative x_in */
static void sign_character_x(int x_in, char ch)
{
    int x = 0, y = 0;
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
                if (x >= 0) {
                    sign_bitmap_set(x, y, status);
                }
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

static void sign_full_bright(void)
{
    uint8_t x, y;

    for (x = 0; x < SIGN_X_MAX; x++) {
        for (y = 0; y < SIGN_Y_MAX; y++) {
            Sign_Bitmap[x][y] = true;
        }
    }
}

static void sign_test_characters(void)
{
    static char ch = ' ';

    sign_clear();
    sign_character_x(0, ch);
    if (ch == 127) {
        ch = ' ';
    } else {
        ch++;
    }
}

static void sign_scroll_character(char ch)
{
    static int x = 0;
    static bool reverse = false;
    int width = 0;
    int end_x = 0;

    sign_clear();
    sign_character_x(x, ch);
    width = font_width(ch);
    end_x -= width;
    if (x < SIGN_X_MAX) {
        if (reverse) {
            if (x == end_x) {
                reverse = false;
            } else {
                x--;
            }
        } else {
            x++;
        }
    } else {
        reverse = true;
        x = SIGN_X_MAX - 1;
    }
}

static void sign_full_bright_blink(void)
{
    static bool clear = false;
    if (clear) {
        sign_clear();
        clear = false;
    } else {
        sign_full_bright();
        clear = true;
    }
}

static void sign_test_bottom(void)
{
    static int x = -8;

    sign_clear();
    sign_character_x(x, '_');
    if (x < SIGN_X_MAX) {
        x++;
    } else {
        x = -8;
    }
}

static void sign_scroll_name(void)
{
    uint8_t i = 0;
    int width = 0;
    int x = 0;
    char ch = ' ';
    unsigned len = 0;
    int end_x = 0;

    sign_clear();
    x = Name_Scroll_X;
    len = strlen(Sign_Text);
    for (i = 0; i < len; i++) {
        ch = Sign_Text[i];
        if (ch == 0) {
            break;
        }
        sign_character_x(x, ch);
        width = font_width(ch);
        end_x -= width;
        x += width;
    }
    if (Name_Scroll_X == end_x) {
        Name_Scroll_X = SIGN_X_MAX - 1;
    } else {
        Name_Scroll_X--;
    }
}

void sign_scroll_name_set(char *name)
{
    sprintf(Sign_Text, "%s", name);
    Name_Scroll_X = SIGN_X_MAX - 1;
}


void sign_task(void)
{
    static SIGN_STATE state = SIGN_SCANNER;

    if (timer_interval_expired(&Timer)) {
        timer_interval_reset(&Timer);
        if (Sign_State != state) {
            /* change of state */
            state = Sign_State;
            switch (state) {
                case SIGN_SCANNER:
                    timer_interval_start(&Timer, 120);
                    break;
                case SIGN_STRING:
                    timer_interval_start(&Timer, 100);
                    break;
                case SIGN_BLINK:
                    timer_interval_start(&Timer, 500);
                    break;
                case SIGN_FULL_BRIGHT:
                    break;
                case SIGN_CLEAR:
                    break;
                default:
                    break;
            }
        }
        switch (state) {
            case SIGN_SCANNER:
                sign_scroll_character(Sign_Character);
                break;
            case SIGN_STRING:
                sign_scroll_name();
                break;
            case SIGN_BLINK:
                sign_full_bright_blink();
                break;
            case SIGN_FULL_BRIGHT:
                sign_full_bright();
                break;
            case SIGN_CLEAR:
                sign_clear();
                break;
            default:
                break;
        }
            
    }
}

/* note: called by interrupt handler */
void sign_timer_handler(void)
{
    static uint8_t last_y = 0;
    static uint8_t last_x = 0;
    static uint8_t y = 0;
    static uint8_t x = 0;

    led_off(last_x, last_y);
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
        led_on(x, y);
    } else {
        led_off(x, y);
    }
    last_y = y;
    last_x = x;
}

void sign_init(void)
{
    timer0_init();
    timer_interval_start(&Timer, 100);
//    sign_scroll_name_set("THE QUICK BROWN FOX JUMPED OVER THE LAZY DOGS");
}
