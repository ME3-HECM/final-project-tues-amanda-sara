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

void whiteLED(unsigned char on_off) {
    redLED = on_off;
    greenLED = on_off;
    blueLED = on_off;
}
