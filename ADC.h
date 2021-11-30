// This is a guard condition so that contents of this file are not included more than once
#ifndef _ADC_H
#define	_ADC_H

#include <xc.h>

#define _XTAL_FREQ 64000000

// Function prototypes
void ADC_init(void);
unsigned char ADC_getval(void);

#endif	/* _ADC_H */