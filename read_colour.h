#ifndef read_colour_H
#define read_colour_H

#include <xc.h>
#include "RGB_LED.h"
#include "color.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

RGB_val read_colour(RGB_val current);
RGB_val read_card(RGB_val current);

#endif
