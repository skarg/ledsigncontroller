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
#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "timer.h"
#include "button.h"

/* stores the value retrieved */
static uint8_t Button;
/* wait for button to stabilize */
static struct itimer Debounce_Timer;
/* bit is set if value has changed */
static bool Change_Of_Value;

bool button_value_changed(void)
{
    return Change_Of_Value;
}

uint8_t button_value(void)
{
    Change_Of_Value = false;
    return Button;
}

void button_value_set(uint8_t value)
{
    if (value != Button) {
        Change_Of_Value = true;
        Button = value;
    }
}

static uint8_t button_value_raw(void)
{
    uint8_t value;

    /* MSB x, x, x, x, x, PC1, PC0, PB5 LSB */
    value = (BITMASK_CHECK(PINC, 0x03) << 1) | (BITMASK_CHECK(PINB, 0x20)>>5);
    value = ~value;
    value = BITMASK_CHECK(value, 0x07);

    return value;
}

void button_task(void)
{
    uint8_t value;
    static uint8_t old_value = 0;

    /* only check the inputs every debounce time */
    if (timer_interval_expired(&Debounce_Timer)) {
        timer_interval_reset(&Debounce_Timer);
        value = button_value_raw();
        if (value == old_value) {
            /* stable at last! */
            button_value_set(value);
        }
        old_value = value;
    }
}

void button_init(
    void)
{
    uint8_t value = 0;

    /* configure the port pin as input */
    BIT_CLEAR(DDRB, DDB5);
    BIT_CLEAR(DDRC, DDC0);
    BIT_CLEAR(DDRC, DDC1);
    /* activate the internal pull up resistors for pin */
    BIT_SET(PORTB, PB5);
    BIT_SET(PORTC, PC0);
    BIT_SET(PORTC, PC1);
    /* configure the debounce timer */
    timer_interval_start(&Debounce_Timer, 80);
    /* initial value */
    value = button_value_raw();
    if (Button != value) {
        Change_Of_Value = true;
        Button = value;
    }
}
