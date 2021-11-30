// This is a guard condition so that contents of this file are not included more than once
#ifndef _interrupts_H
#define	_interrupts_H

#include <xc.h>
#include "dateandtime.h"

#define _XTAL_FREQ 64000000

// This allows use of global variables in other source files)
extern volatile unsigned char sunrise_flag;
extern volatile unsigned char sunset_flag;
extern volatile unsigned char time_flag;

// Function prototypes
void Interrupts_init(dateandtime current);
void __interrupt(high_priority) HighISR();
void __interrupt(low_priority) LowISR();

#endif	/* _interrupts_H */