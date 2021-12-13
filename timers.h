// This is a guard condition so that contents of this file are not included more than once
#ifndef _timers_H
#define	_timers_H

#include <xc.h>
#include "main.h"

#define _XTAL_FREQ 64000000

// This allows auto-switching between "testing" and "normal" modes
#ifdef _TESTING_MODE // "Testing" mode
#define TMR0H_BITS 0b11101110
#define TMR0L_BITS 0b10100011
#define PRESCALER 0b0000

#else // "Normal" mode
#define TMR0H_BITS 0b1011
#define TMR0L_BITS 0b11011011
#define PRESCALER 0b1000
#endif

// Function prototypes
void Timer0_init(void);

#endif	/* _timers_H */