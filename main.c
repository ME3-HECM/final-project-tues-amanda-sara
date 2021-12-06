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
#include "ADC.h"
#include "color.h"
#include "dc_motor.h"
#include "i2c.h"
#include "interrupts.h"
#include "RGB_LED.h"

/***************
 * Main function
 ***************/
void main(void) {
    // Initialisation functions
    color_click_init();
    button_init();
    
    // Colour calibration routine
    
    
    // Motor calibration routine
    
    
    while(1) {
        // Check battery level
        // Monitor the battery voltage via an analogue input pin.
        // The voltage at BAT-VSENSE will always be one third of that at the battery.
    }
}

// Additional LEDs
H.LAMPS; //H.LAMPS turns on the front white LEDs and rear red LEDs, at a reduced brightness.
M.BEAM; //M.BEAM and BRAKE enable you to turn these LEDs on at full brightness.
BRAKE;
TURN-L; //The turn signals have not hardware based brightness control.
TURN-R;