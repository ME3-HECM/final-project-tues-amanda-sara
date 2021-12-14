#ifndef _ADC_H // This is a guard condition so that contents of this file are not included more than once
#define	_ADC_H // Beginning of _ADC_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/*********************
 * Function prototypes
 *********************/
void ADC_init(void);            // Function used to initialise ADC module and set it up to sample BAT-VSENSE on pin RF6
unsigned char ADC_getval(void); // Function used to get current battery voltage level

#endif // End of _ADC_H
