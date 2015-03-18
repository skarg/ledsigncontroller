# Introduction #

This project is an LED sign controller, which controls individual LEDs and displays characters or strings, and does scrolling and blinking.  It is a simple hardware design based on AVR microcontroller.

## Theory of Operation ##

Since the project is designed to run on battery, it needs to use a small amount of power.  The LEDs are lighted only one at a time, very fast, so that the eye sees them all lighted at once.  This is similar to PWM of each LED.  The timer interrupt cycles through each LED and lights it (if it indicates it needs lighted).  At the next timer interrupt, the LED is extinguished and the next LED is lighted.

Each LED is driven by two pins from the microcontroller: one row pin and one column pin.  The row is controlling the negative side of the LEDs.  The column is controlling the positive side of the LED.  To light and LED, the row is turned off, and the column is turned on, while the rest of the rows and columns are set to float (set as an input).

The font based on ASCII table - this site has details:
http://www.asciitable.com/

The font is bitmapped into 8x5 (8 columns, 5 rows), and each font bitmap includes the width (how many pixels wide).  The font includes a single blank column for each character so that no extra spacing is needed for multiple characters (text or strings of characters).