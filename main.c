/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

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
#pragma config WDTE = OFF // WDT operating mode bits (WDT enabled regardless of sleep)

/**************************************************************************************
 * File inclusion makes it easy to handle collections of #defines and declarations etc.
 * See The C programming language, second edition, pp.88-89
 **************************************************************************************/
#include <xc.h> // Include processor files
#include <stdio.h> // Include standard input output library
#include "ADC.h"
#include "color_card.h"
#include "color_click.h"
#include "dc_motor.h"
#include "i2c.h"
#include "serial.h"
#include "interrupts.h"

/***************
 * Main function
 ***************/
void main(void) {
    // Initialisation functions
    ADC_init();
    colorclick_init();
    interrupts_init();
//    DCmotors_init();
    USART4_init();

    // Colour calibration routine
    RGB_val initial; 
    RGB_val current;
    initial = colorclick_readColour(initial); //read ambient light value
    
    // Motor calibration routine
    
    
    while(1) {
        current = colorclick_readColour(current); //read ambient light value
        
        // Testing using serial communication
        char buf[40];
        unsigned int tmpR = current.R;
        unsigned int tmpG = current.G;
        unsigned int tmpB = current.B;
        unsigned int tmpC = current.C;
        sprintf(buf,"%i %i %i %i\n",tmpR,tmpG,tmpB,tmpC);
        sendStringSerial4(buf);
        __delay_ms(500);
    }
}
