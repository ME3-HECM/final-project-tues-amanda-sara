// This is a guard condition so that contents of this file are not included more than once
#ifndef _interrupts_H
#define	_interrupts_H

#include <xc.h>
#include "color_click.h"
#include "i2c.h"

#define _XTAL_FREQ 64000000

// This allows use of global variables in other source files
volatile unsigned char card_flag = 0;          // Declare global variable to toggle when ?
volatile unsigned char battery_flag = 0;             // Declare global variable to toggle when ?

// Function prototypes
void interrupts_init(void);
void __interrupt(high_priority) HighISR();
//void __interrupt(low_priority) LowISR();

#endif	/* _interrupts_H */
