#include <xc.h>
#include <stdio.h>
#include "buttons_n_LEDs.h"
#include "colour_cards.h"
#include "colour_click.h"
#include "DC_motors.h"
#include "serial_comm.h"

/***********************
 * colourcards_readRGBC
 ***********************/
void colourcards_readRGBC(RGBC_val *tmpval, DC_motor *mL, DC_motor *mR)
{
    // Switch off interrupts (to avoid unwanted interrupts while identifying cards)
    PIE0bits.INT1IE = 0;
    
    // Current values at 5 cm distance
    RGB_rel rel;
    colourclick_readRGBC(tmpval);
    colourcards_normaliseRGBC(&abs, &rel);
    
    // Threshold values at 5 cm distance
    // Red/orange
    if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {
        colourclick_readRGBC2(&abs, 3); // Blue LED
        colourcards_normaliseRGBC(&abs, &rel);
        if (rel.B<0.56) {
            // Orange card - Turn right 135 degrees
            turnright(mL, mR, 135);
            stop(mL, mR);
            unknowncard_flag = 0;
        } else {
            // Red card - Turn right 90 degrees
            turnright(mL, mR, 90);
            stop(mL, mR);
            unknowncard_flag = 0;
        }

    // Green/blue/light blue
    } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
        colourclick_readRGBC2(&abs, 3); // Blue LED
        colourcards_normaliseRGBC(&abs, &rel);
        if ((rel.R<0.65) && (rel.B>0.67)) {
            // Blue card - Turn 180 degrees
            turnright(mL, mR, 180);
            stop(mL, mR);
            unknowncard_flag = 0;
        } else if ((rel.R<0.09) && (rel.B>0.62)) {
            // Green card - Turn left 90 degrees
            turnleft(mL, mR, 90);
            stop(mL, mR);
            unknowncard_flag = 0;
        } else {
            // Light blue card - Turn left 135 degrees
            turnleft(mL, mR, 135);
            stop(mL, mR);
            unknowncard_flag = 0;
        }

    // Other colours
    } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {
        // Yellow card - Reverse 1 square and turn right 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        __delay_ms(100);
        turnright(mL, mR, 90);
        stop(mL, mR);
        unknowncard_flag = 0;

    } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        __delay_ms(100);
        turnleft(mL, mR, 90);
        stop(mL, mR);
        unknowncard_flag = 0;

    } else if ((rel.R<0.46) && (rel.G>0.295) && (rel.B>0.21)) {
        // White card - Finish (return home)
        turnright(mL, mR, 180);
        stop(mL, mR);
        unknowncard_flag = 0;
        returnhome_flag = 1;

    } else {
        // Unknown card - Return back to the starting position if final card cannot be found
        __delay_ms(1000);
        forward(mL, mR);
        __delay_ms(10);
        stop(mL, mR);
        colourclick_readRGBC(tmpval);
        if ((tmpval->C < interrupts_lowerbound) || (tmpval->C > interrupts_upperbound)) {
            if (unknowncard_flag<3) {
                PIR0bits.INT1IF = 1;
                unknowncard_flag++;
            } else {
                RH3_LED = 1;
                turnright(mL, mR, 180);
                stop(mL, mR);
                returnhome_flag = !returnhome_flag;
                unknowncard_flag = 0;
            }
        } else {
            forward(mL, mR);
            unknowncard_flag = 0;
        }
    }
    // Switch on interrupts again (to prepare for the next card)
    colourcard_flag = 0;
    PIE0bits.INT1IE = 1;
}

/*********************
 * colourcards_readHSV
 **********************/
void colourcards_readHSV(RGBC_val *tmpval, DC_motor *mL, DC_motor *mR)
{
    
}

/*************************
 * colourcards_testingRGBC
 ************************/
void colourcards_testingRGBC()
{
    INTCONbits.GIE = 0;
    
    while (RF2_BUTTON && RF3_BUTTON);
    MAINBEAM_LED = 1;
    colourclickLEDs_C(1);
    __delay_ms(1000);
    
    RGBC_val abs;
    RGB_rel rel;
    while (1) {
        while (RF2_BUTTON && RF3_BUTTON);
        colourclick_readRGBC(&abs);
        colourcards_normaliseRGBC(&abs, &rel);
        
        char buf[30];
        // Red/orange
        if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {
            colourclick_readRGBC2(&abs, 3); // Blue LED
            colourcards_normaliseRGBC(&abs, &rel);
            if (rel.B<0.56) {
                // Orange card - Turn right 135 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "orange");
            } else {
                // Red card - Turn right 90 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "red");
            }
        
        // Green/blue/light blue
        } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
            colourclick_readRGBC2(&abs, 3); // Blue LED
            colourcards_normaliseRGBC(&abs, &rel);
            if ((rel.R<0.65) && (rel.B>0.67)) {
                // Blue card - Turn 180 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "blue");
            } else if ((rel.R<0.09) && (rel.B>0.62)) {
                // Green card - Turn left 90 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "green");
            } else {
                // Light blue card - Turn left 135 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "light blue");
            }
        
        // Other colours
        } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {
            // Yellow card - Reverse 1 square and turn right 90 degrees
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "yellow");
            
        } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {
            // Pink card - Reverse 1 square and turn left 90 degrees
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "pink");
            
        } else if ((rel.R<0.46) && (rel.G>0.295) && (rel.B>0.21)) {
            // White card - Finish (return home)
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "white");
            
        } else {
            // Unknown card - Return back to the starting position if final card cannot be found
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "unknown");
        }
        sendStringSerial4(buf);
        __delay_ms(500);
    }
}

/************************
 * colourcards_testingHSV
 ************************/
void colourcards_testingHSV(RGBC_val *tmpval)
{
    
}

/***************
 * 
 *******************/
void colourcards_normaliseRGBC(RGBC_val *abs, RGB_rel *rel)
{
    unsigned int R = abs->R;
    unsigned int G = abs->G;
    unsigned int B = abs->B;
    unsigned int C = abs->C;
    
    rel->R = (float)R/(float)C;
    rel->G = (float)G/(float)C;
    rel->B = (float)B/(float)C;
}
