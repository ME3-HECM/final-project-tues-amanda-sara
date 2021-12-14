#include <xc.h>             // Include processor file
#include "buttons_n_LEDs.h" // Include corresponding header file

/****************************************************************
 * clicker2buttons_init
 * Function used to initialise the buttons on the clicker 2 board
 ****************************************************************/
void clicker2buttons_init(void) {
    TRISFbits.TRISF2=1;   // Set TRIS value for RF2 pin (input)
    TRISFbits.TRISF3=1;   // Set TRIS value for RF3 pin (input)
    
    ANSELFbits.ANSELF2=0; // Turn off analogue input on RF2 pin
    ANSELFbits.ANSELF3=0; // Turn off analogue input on RF3 pin
}

/*************************************************************
 * clicker2LEDs_init
 * Function used to initialise the LEDs on the clicker 2 board
 *************************************************************/
void clicker2LEDs_init(void) {
    RD7_LED = 0;          // Set initial LAT value for RD7 LED (low)
    RH3_LED = 0;          // Set initial LAT value for RH3 LED (low)
    
    TRISDbits.TRISD7 = 0; // Set TRIS value for RD7 pin (output)
    TRISHbits.TRISH3 = 0; // Set TRIS value for RH3 pin (output)
}

/*****************************************************************
 * colourclickLEDs_init
 * Function used to initialise the RGB LEDs on the colour click module
 *****************************************************************/
void colourclickLEDs_init(void) {
    colourclickLEDs_C(0); // Set initial LAT values for red, green and blue LEDs
    
    TRISGbits.TRISG1 = 0; // Set TRIS value for red LED (output)
    TRISAbits.TRISA4 = 0; // Set TRIS value for green LED (output)
    TRISFbits.TRISF7 = 0; // Set TRIS value for blue LED (output)
}

/********************************************************************************
 * colorclickLEDs_RGB
 * Function used to cycle the red, green and blue LEDs on the colour click module
 ********************************************************************************/
void colourclickLEDs_RGB(void) {
    RED_LED = 1;    // Switch on the red LED
    __delay_ms(20); // Wait a short delay for human eye to see change
    RED_LED = 0;    // Switch off the red LED
    __delay_ms(20); // Wait a short delay for human eye to see change
    
    GREEN_LED = 1;  // Switch on the green LED
    __delay_ms(20); // Wait a short delay for human eye to see change
    GREEN_LED = 0;  // Switch off the green LED
    __delay_ms(20); // Wait a short delay for human eye to see change
    
    BLUE_LED = 1;   // Switch on the blue LED
    __delay_ms(20); // Wait a short delay for human eye to see change
    BLUE_LED = 0;   // Switch off the blue LED
    __delay_ms(20); // Wait a short delay for human eye to see change
}

/********************************************************************
 * colorclickLEDs_C
 * Function used to toggle the clear light on the colour click module
 ********************************************************************/
void colourclickLEDs_C(unsigned char tog) {
    RED_LED = tog;   // Toggle the red LED
    GREEN_LED = tog; // and the green LED
    BLUE_LED = tog;  // and the blue LED, simultaneously, to give clear light
}

/***************************************************
 * buggyLEDs_init
 * Function used to initialise the LEDs on the buggy
 ***************************************************/
void buggyLEDs_init(void) {
    HEADLAMPS_LED = 0;    // Set initial LAT value for H.LAMPS
    MAINBEAM_LED = 0;     // Set initial LAT value for M.BEAM
    BRAKE_LED = 0;        // Set initial LAT value for BRAKE
    TURNLEFT_LED = 0;     // Set initial LAT value for TURN-L
    TURNRIGHT_LED = 0;    // Set initial LAT value for TURN-R
    
    TRISHbits.TRISH1 = 0; // Set TRIS value for H.LAMPS
    TRISDbits.TRISD3 = 0; // Set TRIS value for M.BEAM
    TRISDbits.TRISD4 = 0; // Set TRIS value for BRAKE
    TRISFbits.TRISF0 = 0; // Set TRIS value for TURN-L
    TRISHbits.TRISH0 = 0; // Set TRIS value for TURN-R
}
