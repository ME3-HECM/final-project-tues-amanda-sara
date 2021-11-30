#include <xc.h>
#include "RGB_LED.h"

void RGB_init(void) {
    TRISGbits.TRISG1 = 0; //red LED
    TRISAbits.TRISA4 = 0; //green LED
    TRISFbits.TRISF7 = 0; //blue LED
    
    LATGbits.LATG1 = 0;
    LATAbits.LATA4 = 0;
    LATFbits.LATF7 = 0;
    
}

void redLED(void){
    LATGbits.LATG1 = !LATGbits.LATG1;
}

void greenLED(void){
    LATAbits.LATA4 = !LATAbits.LATA4;
}

void blueLED(void){
    LATFbits.LATF7 = !LATFbits.LATF7;
}
