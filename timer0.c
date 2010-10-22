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
#include "sign.h"
#include "timer0.h"

#ifndef F_CPU
#error "F_CPU must be defined for Timer configuration."
#endif

/* Timer2 Prescaling: 1, 8, 64, 256, or 1024 */
#define TIMER_MICROSECONDS 200
#define TIMER_TICKS(p) ((((F_CPU)/(p)/1000)*(TIMER_MICROSECONDS))/1000)
#define TIMER_TICKS_MAX 0xff
/* adjust the prescaler for the processor clock */
#if (TIMER_TICKS(1) <= TIMER_TICKS_MAX)
#define TIMER0_PRESCALER 1
#elif (TIMER_TICKS(8) <= TIMER_TICKS_MAX)
#define TIMER0_PRESCALER 8
#elif (TIMER_TICKS(64) <= TIMER_TICKS_MAX)
#define TIMER0_PRESCALER 64
#elif (TIMER_TICKS(256) <= TIMER_TICKS_MAX)
#define TIMER0_PRESCALER 256
#elif (TIMER_TICKS(1024) <= TIMER_TICKS_MAX)
#define TIMER0_PRESCALER 1024
#else
#error "TIMER0: F_CPU too large for timer prescaler."
#endif
#define TIMER0_TICKS TIMER_TICKS(TIMER0_PRESCALER)
/* Timer counts up from count to TIMER_TICKS_MAX and then signals overflow */
#define TIMER0_COUNT (TIMER_TICKS_MAX-TIMER0_TICKS)

/* forward prototype */
ISR(TIMER0_OVF_vect);

/*************************************************************************
* Description: Timer Interrupt Service Routine - Timer Overflowed!
* Returns: none
* Notes: Global interupts must be enabled
*************************************************************************/
ISR(TIMER0_OVF_vect)
{
    /* Set the counter for the next interrupt */
    TCNT0 = TIMER0_COUNT;
    /* do something useful */
    sign_timer_handler();
}

/*************************************************************************
* Description: Initialization for Timer
* Returns: none
* Notes: none
*************************************************************************/
void timer0_init(
    void)
{
    /* Normal Operation - counting up */
    TCCR0A = 0;
    /* Timer2: prescale selections:
       CSn2 CSn1 CSn0 Description
       ---- ---- ---- -----------
       0    0    0  No Clock Source
       0    0    1  No prescaling
       0    1    0  CLKt2s/8
       0    1    1  CLKt2s/32
       1    0    0  CLKt2s/64
       1    0    1  CLKt2s/128
       1    1    0  CLKt2s/256
       1    1    1  CLKt2s/1024
     */
#if (TIMER0_PRESCALER==1)
    TCCR0B = _BV(CS00);
#elif (TIMER0_PRESCALER==8)
    TCCR0B = _BV(CS01);
#elif (TIMER0_PRESCALER==64)
    TCCR0B = _BV(CS01) | _BV(CS00);
#elif (TIMER0_PRESCALER==256)
    TCCR0B = _BV(CS02);
#elif (TIMER0_PRESCALER==1024)
    TCCR0B = _BV(CS02) | _BV(CS00);
#else
#error Timer0 Prescale: Invalid Value
#endif
    /* Clear any TOV Flag set when the timer overflowed - by setting it! */
    BIT_SET(TIFR0, TOV0);
    /* Initial value */
    TCNT0 = TIMER0_COUNT;
    /* Enable the overflow interrupt */
    BIT_SET(TIMSK0, TOIE0);
    /* Clear the Power Reduction Timer/Counter0 */
    BIT_CLEAR(PRR, PRTIM0);
}
