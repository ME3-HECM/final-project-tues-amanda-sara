#ifndef _TIMERS_H // This is a guard condition so that contents of this file are not included more than once
#define	_TIMERS_H // Beginning of _TIMES_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/*******************
 * Timer0 parameters
 *******************/
#define TMR0H_BITS 0b00111100 // Initial value for 16-bit Timer 0 most significant byte
#define TMR0L_BITS 0b10101111 // Initial value for 16-bit Timer 0 least significant byte
#define PRESCALER 0b0101      // Timer0 prescaler value

/*********************
 * Function prototypes
 *********************/
void timer0_init(void); // Function used to set up Timer 0 to track time

#endif // End of _TIMERS_H
