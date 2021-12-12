#ifndef _COLOUR_CLICK_H // This is a guard condition so that contents of this file are not included more than once
#define _COLOUR_CLICK_H // Beginning of colour_click_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/*************
 * RGBC values
 *************/
typedef struct {             // Define a structure
    unsigned int R, G, B, C; // Containing the RGBC values read by the colour click
} RGBC_val;                  // This structure is named RGB_val

/******************
 * Global variables
 ******************/
extern volatile unsigned int interrupts_lower; // Lower threshold value to trigger interrupts
extern volatile unsigned int interrupts_upper; // Upper threshold value to trigger interrupts

/*********************
 * Function prototypes
 *********************/
void colourclick_init(void);
void colourclick_writetoaddr(char address, char value);
unsigned int colourclick_readR(void);
unsigned int colourclick_readG(void);
unsigned int colourclick_readB(void);
unsigned int colourclick_readC(void);
void colourclick_readRGBC(RGBC_val *tmpval);
void colourclick_readRGBC2(RGBC_val *tmpval);
void colourclick_calibration(void);

#endif // End of _COLOUR_CLICK_H
