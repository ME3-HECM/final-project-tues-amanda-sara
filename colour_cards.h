#ifndef _COLOUR_CARDS_H // This is a guard condition so that contents of this file are not included more than once
#define _COLOUR_CARDS_H // Beginning of _colour_cards_H

#include <xc.h>           // Include processor file
#include "main.h"         // Include header file for returnhome_flag global variable
#include "colour_click.h" // Include header file for RGB_val structure
#include "DC_motors.h"    // Include header file for DC_motor structure

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/******************
 * Global variables
 ******************/
extern volatile unsigned char colourcard_flag;  // Toggled when any colour card is encountered
extern volatile unsigned char unknowncard_flag; // Incremented when card cannot be identified
extern volatile unsigned char returnhome_flag;  // Toggled when the final card has been found (i.e. encountered the white card)
                                                // or in exceptional scenarios (i.e. cannot identify a card or stuck in the maze for too long)

/*********************
 * Function prototypes
 *********************/
void colourcards_readRGBC(RGBC_val *tmpval, DC_motor *mL, DC_motor *mR);
void colourcards_readHSV(RGBC_val *tmpval, DC_motor *mL, DC_motor *mR);
void colourcards_testing(void);

#endif // End of _COLOUR_CARDS_H
