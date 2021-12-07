#ifndef _colour_card_H
#define _colour_card_H

#include <xc.h>
#include "color_click.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

RGB_val read_card(RGB_val current);

#endif
