#ifndef _INTERRUPTS_H // This is a guard condition so that contents of this file are not included more than once
#define	_INTERRUPTS_H // Beginning of _INTERRUPTS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

#define AILTL 0x04
#define AILTH 0x05
#define AIHTL 0x06
#define AIHTH 0x07

/******************
 * Global variables
 ******************/
extern volatile unsigned int interrupts_lowerbound; // Lower threshold value to trigger interrupts
extern volatile unsigned int interrupts_upperbound; // Upper threshold value to trigger interrupts
extern volatile unsigned char colourcard_flag;
extern volatile unsigned char battery_flag;

/*********************
 * Function prototypes
 *********************/
void interrupts_init(void);
void interrupts_clear(void);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif // End of _INTERRUPTS_H
