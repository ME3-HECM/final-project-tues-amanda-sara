#ifndef _COLOUR_CLICK_H // This is a guard condition so that contents of this file are not included more than once
#define _COLOUR_CLICK_H // Beginning of colour_click_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/************************
 * RGBC values structures
 ************************/
typedef struct {             // Define a structure
    unsigned int R, G, B, C; // Containing the RGBC values read by the colour click
} RGBC_val;                  // Name this structure RGBC_val

typedef struct {   // Define a structure
    float R, G, B; // Containing the RGB values normalised by C
} RGB_rel;         // Name this structure RGB_rel

/******************
 * Global variables
 ******************/
extern volatile unsigned int interrupts_lowerbound; // Lower clear threshold value to trigger interrupts when encounter colour cards (refer main.h)
extern volatile unsigned int interrupts_upperbound; // Upper clear threshold value to trigger interrupts when encounter colour cards (refer main.h)

/*********************
 * Function prototypes
 *********************/
void colourclick_init(void);                                      // Function used to initialise the colour click module using I2C
void colourclick_writetoaddr(char address, char value);           // Function used to write to the colour click module
unsigned int colourclick_readR(void);                             // Function used to read the red channel and return a 16 bit ADC value representing colour intensity
unsigned int colourclick_readG(void);                             // Function used to read the green channel and return a 16 bit ADC value representing colour intensity
unsigned int colourclick_readB(void);                             // Function used to read the blue channel and return a 16 bit ADC value representing colour intensity
unsigned int colourclick_readC(void);                             // Function used to read the clear channel and return a 16 bit ADC value representing colour intensity
void colourclick_readRGBC(RGBC_val *tmpval);                      // Function used to read the RGBC values using clear light
void colourclick_readRGBC2(RGBC_val *tmpval, unsigned char mode); // Function used to read the RGBC values using the red, green or blue LEDs
void colourclick_calibration(void);                               // Function used to calibrate the colour click module
void colourclick_testing(RGBC_val *initval, RGBC_val *tmpval);    // Function used to test the colour click module

#endif // End of _COLOUR_CLICK_H
