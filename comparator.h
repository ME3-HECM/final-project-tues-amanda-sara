#ifndef _COMPARATOR_H // This is a guard condition so that contents of this file are not included more than once
#define _COMPARATOR_H // Beginning of _COMPARATOR_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/*********************
 * Function prototypes
 *********************/
void DAC_init(void);
void Comp1_init(void);

#endif // End of _COMPARATOR_H
