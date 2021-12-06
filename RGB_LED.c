#include <xc.h>
#include "RGB_LED.h"

/**********************
 * RGB_init
 * Function used to
 *************************/
void RGB_init(void) {
    TRISGbits.TRISG1 = 0; //red LED
    TRISAbits.TRISA4 = 0; //green LED
    TRISFbits.TRISF7 = 0; //blue LED
    
    LATGbits.LATG1 = 0;
    LATAbits.LATA4 = 0;
    LATFbits.LATF7 = 0;
}

/*****************
 * redLED
 * Function used to
 *************************/
void redLED(void){
    LATGbits.LATG1 = !LATGbits.LATG1;
}

/****************
 * greenLED
 * Function used to
 **************************/
void greenLED(void){
    LATAbits.LATA4 = !LATAbits.LATA4;
}

/*****************
 * blueLED
 * Function used to
 ***********************/
void blueLED(void){
    LATFbits.LATF7 = !LATFbits.LATF7;
}
