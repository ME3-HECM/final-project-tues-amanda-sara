#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#include "colour_cards.h"   // Include corresponding header file
#include "buttons_n_LEDs.h" // Include header file to use buttons and LEDs on the clicker 2 board
#include "colour_click.h"   // Include header file to use colour click module for colour identication purposes
#include "DC_motors.h"      // Include header file to control DC motors based on identified colour cards
#include "serial_comm.h"    // Include header file to use serial communication module to output results on-screen for testing purposes

/*******************************************************************
 * colourcards_readRGBC
 * Function used to identify the colour card and respond accordingly
 *******************************************************************/
void colourcards_readRGBC(RGBC_val *abs, DC_motor *mL, DC_motor *mR) {
    PIE0bits.INT1IE = 0; // Prevent unwanted interrupts from the colour click module
    
    RGB_rel rel;                                         //
    colourclick_readRGBC(abs);                           //
    colourcards_normaliseRGBC(abs, &rel);                //
    
    // Category 1: Red/orange cards (very similar in colour)
    if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {
        colourclick_readRGBC2(abs, 3);         // Blue LED
        colourcards_normaliseRGBC(abs, &rel);  //
        if (rel.G<0.185) {
            // Red card - Turn right 90 degrees
            instructions(mL, mR, 1);
        } else {
            // Orange card - Turn right 135 degrees
            instructions(mL, mR, 6);
        }

    // Category 2: Green/blue/light blue cards (very similar in colour)
    } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
        colourclick_readRGBC2(abs, 1); // Red LED
        colourcards_normaliseRGBC(abs, &rel);
        if (rel.B<0.125) {
            // Green card - Turn left 90 degrees
            instructions(mL, mR, 2);
        } else {
            colourclick_readRGBC2(abs, 2); // Green LED
            colourcards_normaliseRGBC(abs, &rel);
            if (rel.R<0.115) {
                // Blue card - Turn 180 degrees
                instructions(mL, mR, 3);
            } else {
                // Light blue card - Turn left 135 degrees
                instructions(mL, mR, 7);
            }
        }
    // Category 3: Other cards (distinct and easy to identify)
    } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {
        // Yellow card - Reverse 1 square and turn right 90 degrees
        instructions(mL, mR, 4);

    } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        instructions(mL, mR, 5);

    } else if ((rel.R<0.47) && (rel.G>0.295) && (rel.B>0.21)) {
        // White card - Finish (return home)
        unknowncard_flag = 0;
        returnhome_flag = 1;

    } else {
        // Unknown card - Return back to the starting position if final card cannot be found
        __delay_ms(1000);
        forward(mL, mR);
        __delay_ms(10);
        stop(mL, mR);
        colourclick_readRGBC(abs);
        if ((abs->C < interrupts_lowerbound) || (abs->C > interrupts_upperbound)) {
            if (unknowncard_flag<3) {
                unknowncard_flag++;
                PIR0bits.INT1IF = 1;
            } else {
                RH3_LED = 1;
                returnhome_flag = 1;
            }
        } else {unknowncard_flag = 0;}
    }
    
    // Switch on interrupts again (to prepare for the next card)
    PIE0bits.INT1IE = 1;
}

/********************************************************************************
 * colourcards_testingRGBC
 * Function used to identify colours and respond accordingly for testing purposes
 ********************************************************************************/
void colourcards_testingRGBC(RGBC_val *tmpval, DC_motor *mL, DC_motor *mR) {
    MAINBEAM_LED = 1;
    colourclickLEDs_C(1);
    
    while(1) {
        while(RF2_BUTTON && RF3_BUTTON);
        __delay_ms(50);
        colourcards_readRGBC(tmpval, mL, mR);
    }
}

/******************************************************************************************************
 * colourcards_testingRGBC2
 * Function used to identify colours and output results using serial communication for testing purposes
 ******************************************************************************************************/
void colourcards_testingRGBC2() {
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
        
        char buf[100];
        // Red/orange
        if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {
            colourclick_readRGBC2(&abs, 3); // Blue LED
            colourcards_normaliseRGBC(&abs, &rel);
            if (rel.G<0.185) {
                // Red card - Turn right 90 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "red");
            } else {
                // Orange card - Turn right 135 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "orange");
            }
        // Green/blue/light blue
        } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
            colourclick_readRGBC2(&abs, 1); // Red LED
            colourcards_normaliseRGBC(&abs, &rel);
            if (rel.B<0.125) {
                // Green card - Turn left 90 degrees
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "green");
            } else {
                colourclick_readRGBC2(&abs, 2); // Green LED
                colourcards_normaliseRGBC(&abs, &rel);
                if (rel.R<0.115) {
                    // Blue card - Turn 180 degrees
                    sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                            abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "blue");
                } else {
                    // Light blue card - Turn left 135 degrees
                    sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                            abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "light blue");
                }
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
            
        } else if ((rel.R<0.47) && (rel.G>0.295) && (rel.B>0.21)) {
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

/********************************************
 * colourcards_normaliseRGBC
 * Function used to normalise the RGBC values
 ********************************************/
void colourcards_normaliseRGBC(RGBC_val *abs, RGB_rel *rel) {
    unsigned int R = abs->R; // 
    unsigned int G = abs->G; // 
    unsigned int B = abs->B; // 
    unsigned int C = abs->C; // 
    
    rel->R = (float)R/(float)C; // 
    rel->G = (float)G/(float)C; // 
    rel->B = (float)B/(float)C; // 
}
