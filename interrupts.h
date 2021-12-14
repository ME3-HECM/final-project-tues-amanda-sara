#ifndef _INTERRUPTS_H // This is a guard condition so that contents of this file are not included more than once
#define	_INTERRUPTS_H // Beginning of _INTERRUPTS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/******************
 * Global variables
 ******************/
extern volatile unsigned int tmp;
volatile unsigned int instr[20];
volatile unsigned int dur[20];
volatile unsigned char instr_counter;
volatile unsigned char dur_counter;
extern volatile unsigned int interrupts_lowerbound; // Lower clear threshold value to trigger interrupts when encounter colour cards
extern volatile unsigned int interrupts_upperbound; // Upper clear threshold value to trigger interrupts when encounter colour cards
extern volatile unsigned char colourcard_flag;      // Toggled when buggy encounters a colour card
extern volatile unsigned char returnhome_flag;      // Toggled when buggy has found the final white card or in exceptions

/*********************
 * Function prototypes
 *********************/
void interrupts_init(void);                // Function used to turn on and set priority for interrupts to detect colour cards and track time
void interrupts_clear(void);               // Function used to clear interrupts on the colour click module
void interrupts_colourclick(void);         // Function used to set up interrupts on the colour click module
void __interrupt(high_priority) HighISR(); // Special function for high priority interrupt service routine when approaching colour cards
void __interrupt(low_priority) LowISR();   // Special function for low priority interrupt service routine when timer 0 overflows

#endif // End of _INTERRUPTS_H
