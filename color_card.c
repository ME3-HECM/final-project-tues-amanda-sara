#include <xc.h>
#include "color_click.h"

RGB_val read_card(RGB_val current) {
    redLED = 1;
    current.R = colorclick_readRed();
    __delay_ms(1000);
    redLED = 0;
    __delay_ms(20);
    
    greenLED = 1;
    current.G = colorclick_readGreen();
    __delay_ms(1000);
    greenLED = 0;
    __delay_ms(20);
    
    blueLED = 1;
    current.B = colorclick_readBlue();
    __delay_ms(1000);
    blueLED = 0;
    __delay_ms(20);
    
    return current;
}

void check_red(unsigned int initial, unsigned int current) {
    unsigned int diff;
    diff = current - initial;
    if(diff>150) LATHbits.LATH3 = !LATHbits.LATH3;
}
