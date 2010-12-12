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
#include "init.h"
#include "timer.h"
#include "led.h"
#include "sign.h"
#include "button.h"
#include "watchdog.h"

static char *Phrases[] = {
    "DAFT PUNK",
    "HUMAN AFTER ALL",
    "WORK IT  MAKE IT  DO IT    MAKES US"
    "HARDER   BETTER   FASTER   STRONGER",
    "ROCK   ROBOT ROCK",
    "--+--+--+--+",
    "``````",
    "HELLO WORLD",
    "GO CAVS",
    "TELEVISION RULES THE NATION",
    NULL
};

static void main_task(void)
{
    static unsigned phrase_index = 0;
    static unsigned full_bright = 0;
    uint8_t value;
    char *phrase = NULL;

    if (button_value_changed()) {
        value = button_value();
        if (BIT_CHECK(value, 1)) {
            phrase = Phrases[phrase_index];
            if (phrase == NULL) {
                phrase_index = 0;
                phrase = Phrases[0];
            } else {
                phrase_index++;
            }
            sign_scroll_name_set(phrase);
            sign_state_set(SIGN_STRING);
        } else if (BIT_CHECK(value, 2)) {
            if (full_bright == 0) {
                full_bright++;
                sign_character_set('=');
                sign_state_set(SIGN_SCANNER);
            } else if (full_bright == 1) {
                full_bright++;
                sign_character_set('-');
                sign_state_set(SIGN_SCANNER);
            } else if (full_bright == 2) {
                full_bright++;
                sign_character_set('*');
                sign_state_set(SIGN_SCANNER);
            } else if (full_bright == 3) {
                full_bright++;
                sign_character_set(127);
                sign_state_set(SIGN_SCANNER);
            } else {
                full_bright = 0;
                sign_state_set(SIGN_FULL_BRIGHT);
            }
        } else if (BIT_CHECK(value, 0)) {
            sign_state_set(SIGN_BLINK);
        }
    }
}

int main(
    void)
{
    init();
    /* Configure the watchdog timer - Disabled for debugging */
#ifdef NDEBUG
    watchdog_init(2000);
#else
    watchdog_init(0);
#endif
    timer_init();
    led_init();
    button_init();
    sign_init();
    /* Enable global interrupts */
    __enable_interrupt();
    for (;;) {
        watchdog_reset();
        button_task();
        sign_task();
        main_task();
    }
}
