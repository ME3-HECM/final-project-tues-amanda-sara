/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000         // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/***************************************************
 * CONFIG1L (configuration word 1) - oscillators
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.28-29
 ***************************************************/
#pragma config FEXTOSC = HS         // FEXTOSC external oscillator mode selection bits (HS (crystal oscillator) above 8 MHz; PFM (program memory) set to high power)
#pragma config RSTOSC = EXTOSC_4PLL // Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

/***********************************************************
 * CONFIG3L (configuration word 3) - windowed watchdog timer
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.32-33
 ***********************************************************/
#pragma config WDTE = OFF           // WDT operating mode bits (WDT enabled regardless of sleep)

/**************************************************************************************
 * File inclusion makes it easy to handle collections of #defines and declarations etc.
 * See The C programming language, second edition, pp.88-89
 **************************************************************************************/
#include <xc.h>                     // Include processor files
#include "color.h"
#include "RGB_LED.h"
#include "dc_motor.h"

/***************
 * Main function
 ***************/
void main(void) {
    color_click_init();
}
