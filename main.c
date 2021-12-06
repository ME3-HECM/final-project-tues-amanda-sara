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
#include <stdio.h>
#include "ADC.h"
#include "color.h"
#include "dc_motor.h"
#include "i2c.h"
#include "interrupts.h"
#include "RGB_LED.h"
#include "read_colour.h"
#include "serial.h"


/***************
 * Main function
 ***************/
void main(void) {
    // Initialisation functions
    I2C_2_Master_Init();
    color_click_init();
    RGB_init();
    initUSART4();
//    button_init();
    
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;

    // Colour calibration routine
    whiteLED(1);
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD3 = 1;
    
    RGB_val initial; 
//    initial = read_colour(initial); //read ambient light value
    
    // Motor calibration routine
    RGB_val current;
    
    while(1) {
        
        initial = read_colour(initial); //read ambient light value
        
        char buf[40];
        unsigned int tmpR = initial.R;
        unsigned int tmpG = initial.G;
        unsigned int tmpB = initial.B;
        unsigned int tmpC = initial.C;
        sprintf(buf,"%i %i %i %i\n",tmpR,tmpG,tmpB,tmpC);
        sendStringSerial4(buf);
        __delay_ms(500);      

        // Check battery level
        // Monitor the battery voltage via an analogue input pin.
        // The voltage at BAT-VSENSE will always be one third of that at the battery.
    }
}

// Additional LEDs
//H.LAMPS; //H.LAMPS turns on the front white LEDs and rear red LEDs, at a reduced brightness.
//M.BEAM; //M.BEAM and BRAKE enable you to turn these LEDs on at full brightness.
//BRAKE;
//TURN-L; //The turn signals have not hardware based brightness control.
//TURN-R;