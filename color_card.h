#ifndef _colour_card_H
#define _colour_card_H

#include <xc.h>
#include "color_click.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

volatile unsigned char returnhome_flag;

RGB_val read_colour(RGB_val current);
void read_card(RGB_val initial, RGB_val current, DC_motor *mL, DC_motor *mR);
unsigned char stop_check(RGB_val current);

#endif
