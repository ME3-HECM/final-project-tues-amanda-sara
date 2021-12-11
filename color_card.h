#ifndef _colour_card_H
#define _colour_card_H

#include <xc.h>
#include "color_click.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

volatile unsigned char returnhome_flag;

void read_card_RGB(RGB_val current, DC_motor *mL, DC_motor *mR);

#endif
