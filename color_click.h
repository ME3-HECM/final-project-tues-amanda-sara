#ifndef _color_click_H
#define _color_click_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define redLED LATGbits.LATG1 
#define greenLED LATAbits.LATA4
#define blueLED LATFbits.LATF7

//Definition of RGB structure
typedef struct {
    unsigned int R, G, B, C;
} RGB_val;

// Function prototypes
void colorclick_init(void);
void colorclick_cyclingRGBLED(void);
void colorclick_toggleClearLED(unsigned char toggle);
void colorclick_writetoaddr(char address, char value);
unsigned int colorclick_readRed(void);
unsigned int colorclick_readGreen(void);
unsigned int colorclick_readBlue(void);
unsigned int colorclick_readClear(void);
RGB_val colorclick_readColour(RGB_val current);

#endif
