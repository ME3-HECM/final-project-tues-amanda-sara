#include <xc.h>             // Include processor file
#include "buttons_n_LEDs.h" // Include corresponding header file

/****************************************************************
 * clicker2buttons_init
 * Function used to initialise the buttons on the clicker 2 board
 ****************************************************************/
void clicker2buttons_init(void) {
    // setup pin for input (connected to button)
    TRISFbits.TRISF2=1; //set TRIS value for RF2 pin (input)
    TRISFbits.TRISF3=1; //set TRIS value for RF3 pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on RF2 pin
    ANSELFbits.ANSELF3=0; //turn off analogue RF3 input on pin
}

/*************************************************************
 * clicker2LEDs_init
 * Function used to initialise the LEDs on the clicker 2 board
 *************************************************************/
void clicker2LEDs_init(void) {
    TRISDbits.TRISD7 = 0;
    TRISHbits.TRISH3 = 0;
    
    RD7_LED = 0;
    RH3_LED = 0;
}

/*****************************************************************
 * colourclickLEDs_init
 * Function used to initialise the LEDs on the colour click module
 *****************************************************************/
void colourclickLEDs_init(void) {
    //set TRIS values
    TRISGbits.TRISG1 = 0; //red LED
    TRISAbits.TRISA4 = 0; //green LED
    TRISFbits.TRISF7 = 0; //blue LED
    
    //set LAT values
    colourclickLEDs_C(0);
}

/********************************************************************************
 * colorclickLEDs_RGB
 * Function used to cycle the red, green and blue LEDs on the colour click module
 ********************************************************************************/
void colourclickLEDs_RGB(void) {
    RED_LED = 1;
    __delay_ms(20);
    RED_LED = 0;
    __delay_ms(20);
    
    GREEN_LED = 1;
    __delay_ms(20);
    GREEN_LED = 0;
    __delay_ms(20);
    
    BLUE_LED = 1;
    __delay_ms(20);
    BLUE_LED = 0;
    __delay_ms(20);
}

/********************************************************************
 * colorclickLEDs_C
 * Function used to toggle the clear light on the colour click module
 ********************************************************************/
void colourclickLEDs_C(unsigned char tog) {
    RED_LED = tog;
    GREEN_LED = tog;
    BLUE_LED = tog;
}

/***************************************************
 * buggyLEDs_init
 * Function used to initialise the LEDs on the buggy
 ***************************************************/
void buggyLEDs_init(void) {
    TRISHbits.TRISH1 = 0; // H.LAMPS
    TRISDbits.TRISD3 = 0; // M.BEAM
    TRISDbits.TRISD4 = 0; // BRAKE
    TRISFbits.TRISF0 = 0; // TURN-L
    TRISHbits.TRISH0 = 0; // TURN-R
    
    HEADLAMPS_LED = 0;
    MAINBEAM_LED = 0;
    BRAKE_LED = 0;
    TURNLEFT_LED = 0;
    TURNRIGHT_LED = 0;
}
