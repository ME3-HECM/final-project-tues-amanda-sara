#ifndef _ADC_H // This is a guard condition so that contents of this file are not included more than once
#define	_ADC_H // Beginning of _ADC_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/*********************
 * Function prototypes
 *********************/
void ADC_init(void);
unsigned char ADC_getval(void);

#endif // End of _ADC_H
