#ifndef _RGB_LED_H
#define	_RGB_LED_H

#define redLED LATGbits.LATG1 
#define greenLED LATAbits.LATA4
#define blueLED LATFbits.LATF7

#include <xc.h>   

void RGB_init(void);
void whiteLED(unsigned char on_off);

#endif
