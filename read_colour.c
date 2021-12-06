/*
 * File:   read_colour.c
 * Author: s
 *
 * Created on December 6, 2021, 10:06 AM
 */


#include <xc.h>
#include "RGB_LED.h"
#include "color.h"

#define _XTAL_FREQ 64000000  

RGB_val read_colour(RGB_val current) {
    
    current.R = color_read_Red();
    current.G = color_read_Green();
    current.B = color_read_Blue();
    current.C = color_read_Clear();

    return current;
}

RGB_val read_card(RGB_val current) {
    
    redLED = 1;
    current.R = color_read_Red();
    __delay_ms(1000);
    redLED = 0;
    
    greenLED = 1;
    current.G = color_read_Green();
    __delay_ms(1000);
    greenLED = 0;
    
    blueLED = 1;
    current.B = color_read_Blue();
    __delay_ms(1000);
    blueLED = 0;
    
    return current;
}

void check_red(unsigned int initial, unsigned int current) {
    unsigned int diff
    diff = current - initial;
    if(diff>150) LATHbits.LATH3 = !LATHbits.LATH3;
}


