#ifndef _COLOUR_CARDS_H // This is a guard condition so that contents of this file are not included more than once
#define _COLOUR_CARDS_H // Beginning of _colour_cards_H

#include <xc.h>           // Include processor file
#include "main.h"         // Include header file for global variables
#include "colour_click.h" // Include header file for RGBC_val and RGB_rel structures
#include "DC_motors.h"    // Include header file for DC_motor structure

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/******************
 * Global variables
 ******************/
extern volatile unsigned char colourcard_flag;  // Toggled when buggy encounters a colour card (refer main.h)
extern volatile unsigned char unknowncard_flag; // Incremented each time the buggy fails to identify a colour card (refer main.h)
extern volatile unsigned char returnhome_flag;  // Toggled when buggy has found the final white card or in exceptions (refer main.h)

/*********************
 * Function prototypes
 *********************/
void colourcards_readRGBC(RGBC_val *abs, DC_motor *mL, DC_motor *mR); // Function used to identify the colour card and respond accordingly
void colourcards_testingRGBC();                                       // Function used to identify colours and output results using serial communication for the testing purposes
void colourcards_normaliseRGBC(RGBC_val *abs, RGB_rel *rel);          // Function used to normalise the RGBC values

#endif // End of _COLOUR_CARDS_H
