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
#ifndef SIGN_H
#define SIGN_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SIGN_SCANNER = 0,
    SIGN_STRING = 1,
    SIGN_BLINK = 2,
    SIGN_FULL_BRIGHT = 3,
    SIGN_CLEAR = 4,
    SIGN_STATE_MAX = 5
} SIGN_STATE;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void sign_init(void);
void sign_task(void);
void sign_timer_handler(void);
void sign_character_set(char ch);
void sign_scroll_name_set(char *name);
void sign_clear(void);
void sign_state_set(SIGN_STATE state);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
