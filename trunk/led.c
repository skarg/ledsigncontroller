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
#include "hardware.h"
#include "timer.h"
#include "led.h"

/*************************************************************************
* Description: Turn on an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_row_on(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_SET(DDRB, DDB4);
            BIT_SET(PORTB, PB4);
            break;
        case 1:
            BIT_SET(DDRB, DDB3);
            BIT_SET(PORTB, PB3);
            break;
        case 2:
            BIT_SET(DDRB, DDB2);
            BIT_SET(PORTB, PB2);
            break;
        case 3:
            BIT_SET(DDRB, DDB1);
            BIT_SET(PORTB, PB1);
            break;
        case 4:
            BIT_SET(DDRB, DDB0);
            BIT_SET(PORTB, PB0);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn on an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_row_off(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_SET(DDRB, DDB4);
            BIT_CLEAR(PORTB, PB4);
            break;
        case 1:
            BIT_SET(DDRB, DDB3);
            BIT_CLEAR(PORTB, PB3);
            break;
        case 2:
            BIT_SET(DDRB, DDB2);
            BIT_CLEAR(PORTB, PB2);
            break;
        case 3:
            BIT_SET(DDRB, DDB1);
            BIT_CLEAR(PORTB, PB1);
            break;
        case 4:
            BIT_SET(DDRB, DDB0);
            BIT_CLEAR(PORTB, PB0);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn off an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_row_float(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_CLEAR(PORTB, PB4);
            BIT_CLEAR(DDRB, DDB4);
            break;
        case 1:
            BIT_CLEAR(PORTB, PB3);
            BIT_CLEAR(DDRB, DDB3);
            break;
        case 2:
            BIT_CLEAR(PORTB, PB2);
            BIT_CLEAR(DDRB, DDB2);
            break;
        case 3:
            BIT_CLEAR(PORTB, PB1);
            BIT_CLEAR(DDRB, DDB1);
            break;
        case 4:
            BIT_CLEAR(PORTB, PB0);
            BIT_CLEAR(DDRB, DDB0);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn on an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_column_off(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_SET(DDRA, DDA0);
            BIT_CLEAR(PORTA, PA0);
            break;
        case 1:
            BIT_SET(DDRA, DDA1);
            BIT_CLEAR(PORTA, PA1);
            break;
        case 2:
            BIT_SET(DDRA, DDA2);
            BIT_CLEAR(PORTA, PA2);
            break;
        case 3:
            BIT_SET(DDRA, DDA3);
            BIT_CLEAR(PORTA, PA3);
            break;
        case 4:
            BIT_SET(DDRA, DDA4);
            BIT_CLEAR(PORTA, PA4);
            break;
        case 5:
            BIT_SET(DDRA, DDA5);
            BIT_CLEAR(PORTA, PA5);
            break;
        case 6:
            BIT_SET(DDRA, DDA6);
            BIT_CLEAR(PORTA, PA6);
            break;
        case 7:
            BIT_SET(DDRA, DDA7);
            BIT_CLEAR(PORTA, PA7);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn on an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_column_on(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_SET(DDRA, DDA0);
            BIT_SET(PORTA, PA0);
            break;
        case 1:
            BIT_SET(DDRA, DDA1);
            BIT_SET(PORTA, PA1);
            break;
        case 2:
            BIT_SET(DDRA, DDA2);
            BIT_SET(PORTA, PA2);
            break;
        case 3:
            BIT_SET(DDRA, DDA3);
            BIT_SET(PORTA, PA3);
            break;
        case 4:
            BIT_SET(DDRA, DDA4);
            BIT_SET(PORTA, PA4);
            break;
        case 5:
            BIT_SET(DDRA, DDA5);
            BIT_SET(PORTA, PA5);
            break;
        case 6:
            BIT_SET(DDRA, DDA6);
            BIT_SET(PORTA, PA6);
            break;
        case 7:
            BIT_SET(DDRA, DDA7);
            BIT_SET(PORTA, PA7);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn on an LED row
* Returns: none
* Notes: none
*************************************************************************/
static void led_column_float(
    uint8_t index)
{
    switch (index) {
        case 0:
            BIT_CLEAR(PORTA, PA0);
            BIT_CLEAR(DDRA, DDA0);
            break;
        case 1:
            BIT_CLEAR(PORTA, PA1);
            BIT_CLEAR(DDRA, DDA1);
            break;
        case 2:
            BIT_CLEAR(PORTA, PA2);
            BIT_CLEAR(DDRA, DDA2);
            break;
        case 3:
            BIT_CLEAR(PORTA, PA3);
            BIT_CLEAR(DDRA, DDA3);
            break;
        case 4:
            BIT_CLEAR(PORTA, PA4);
            BIT_CLEAR(DDRA, DDA4);
            break;
        case 5:
            BIT_CLEAR(PORTA, PA5);
            BIT_CLEAR(DDRA, DDA5);
            break;
        case 6:
            BIT_CLEAR(PORTA, PA6);
            BIT_CLEAR(DDRA, DDA6);
            break;
        case 7:
            BIT_CLEAR(PORTA, PA7);
            BIT_CLEAR(DDRA, DDA7);
            break;
        default:
            break;
    }
}

/*************************************************************************
* Description: Turn on an LED
* Returns: none
* Notes: none
*************************************************************************/
void led_on(
    uint8_t row,
    uint8_t column)
{
    led_row_off(row);
    led_column_on(column);
}

/*************************************************************************
* Description: Turn off an LED
* Returns: none
* Notes: none
*************************************************************************/
void led_off(
    uint8_t row,
    uint8_t column)
{
    led_row_float(row);
    led_column_float(column);
}

/*************************************************************************
* Description: Initialize the LED hardware
* Returns: none
* Notes: none
*************************************************************************/
void led_init(
    void)
{
}
