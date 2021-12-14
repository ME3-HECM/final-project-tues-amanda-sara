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
    PIE0bits.INT1IE = 0;                                    // Prevent unwanted interrupts from the colour click module
    
    RGB_rel rel;                                            // initialise RGB_rel to store normalised values 
    colourclick_readRGBC(abs);                              // read values of 
    colourcards_normaliseRGBC(abs, &rel);                   // normalises values and stores in rel
    
    // Category 1: Red/orange cards (very similar in colour)
    if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {        // range for red and orange 
        colourclick_readRGBC2(abs, 3);                          // turns one blue LED and reads values 
        colourcards_normaliseRGBC(abs, &rel);                   // normalises blue readings 
        if (rel.G<0.185) {                                      // if green channel for blue flash in range for red
            // Red card - Turn right 90 degrees
            instructions(mL, mR, 1);                            // execute red instruction
        } else {
            // Orange card - Turn right 135 degrees
            instructions(mL, mR, 6);                            // execute orange instruction
        }

    // Category 2: Green/blue/light blue cards (very similar in colour)
    } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) { // range for green/blue/light blue
        colourclick_readRGBC2(abs, 1);                          // flash red LED and records values     
        colourcards_normaliseRGBC(abs, &rel);                   // normalised values from red LED reading
        if (rel.B<0.125) {                                      // blue change range for green card
            // Green card - Turn left 90 degrees
            instructions(mL, mR, 2);                            // execute green instruction
        } else {                                                // need to figure out whether blue or light blue
            colourclick_readRGBC2(abs, 2);                      // flash green LED and record values  
            colourcards_normaliseRGBC(abs, &rel);               // normalise values from green LED reading
            if (rel.R<0.115) {                                  // if red channel reading<0.115 then blue
                // Blue card - Turn 180 degrees
                instructions(mL, mR, 3);                        // execute blue instruction
            } else {
                // Light blue card - Turn left 135 degrees
                instructions(mL, mR, 7);                        //execute light blue instruction
            }
        }
    // Category 3: Other cards (distinct and easy to identify)
    } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {     // range for yellow card
        // Yellow card - Reverse 1 square and turn right 90 degrees
        instructions(mL, mR, 4);                                    // execute yellow instruction

    } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {    // range for pink card
        // Pink card - Reverse 1 square and turn left 90 degrees
        instructions(mL, mR, 5);                                    // execute pink instruction
        
    } else if ((rel.R<0.47) && (rel.G>0.295) && (rel.B>0.21)) {     // range for white card
        // White card - Finish (return home)
        unknowncard_flag = 0;                                       // clear unknown flag in case the card was read on 2nd/3rd attempt
        returnhome_flag = 1;                                        // signal buggy to start return home routine

    } else {
        // Unknown card - Return back to the starting position if final card cannot be found
        __delay_ms(1000);                           // pause
        forward(mL, mR);                            // move forward small amount
        __delay_ms(10);
        stop(mL, mR);                               // stop
        colourclick_readRGBC(abs);                  // take RGBC reading
        if ((abs->C < interrupts_lowerbound) || (abs->C > interrupts_upperbound)) {     // compare reading with interrupt trigger values to see if at a card
            if (unknowncard_flag<3) {               // if less than 3 attempts
                unknowncard_flag++;                 // increment attempts
                PIR0bits.INT1IF = 1;                // turn on interrupt to run read colour sequence again 
            } else {                                // if more than 3 attempts
                RH3_LED = 1;                        
                returnhome_flag = 1;                // start return home routine
            }
        } else {unknowncard_flag = 0;}              // if not at a card, clear flag due to false trigger and try again
    }
    
    PIE0bits.INT1IE = 1;                            // Switch on interrupts again (to prepare for the next card)  
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
    while (RF2_BUTTON && RF3_BUTTON);   // wait for button press
    MAINBEAM_LED = 1;                   // turn on front lights
    colourclickLEDs_C(1);               // turn on clicker white LED
    __delay_ms(1000);                   // wait 
    
    RGBC_val abs;                       // initialise variable for RGB absolute values 
    RGB_rel rel;                        // initialise variables for normalise values
    while (1) {                         // infinite while loop
        while (RF2_BUTTON && RF3_BUTTON);       // wait for button press
        colourclick_readRGBC(&abs);             // read RGBC values 
        colourcards_normaliseRGBC(&abs, &rel);  // normalise values 
        
        char buf[100];                  // initialise to display characters
        // Red/orange
        if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {    // range for red and orange
            colourclick_readRGBC2(&abs, 3);                     // turns one blue LED and reads values
            colourcards_normaliseRGBC(&abs, &rel);              // normalises blue readings
            if (rel.G<0.185) {                                  // if green channel for blue flash in range for red
                // Red card - Turn right 90 degrees
                // display absolute values, normalised values, and print the colour of the card
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "red");
            } else {                                            // otherwise card is orange
                // Orange card - Turn right 135 degrees
                // display absolute values, normalised values, and print the colour of the card
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "orange");
            }
        // Green/blue/light blue                                // range for green/blue/light blue
        } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
            colourclick_readRGBC2(&abs, 1);                     // flash red LED and records values
            colourcards_normaliseRGBC(&abs, &rel);              // normalised values from red LED reading
            if (rel.B<0.125) {                                  // blue channel range for green card
                // Green card - Turn left 90 degrees
                // display absolute values, normalised values, and print the colour of the card
                sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                        abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "green");
            } else {                                            // need to figure out whether blue or light blue
                colourclick_readRGBC2(&abs, 2);                 // flash green LED and record values
                colourcards_normaliseRGBC(&abs, &rel);          // normalise values from green LED reading
                if (rel.R<0.115) {                              // red channel range for blue card
                    // Blue card - Turn 180 degrees
                    // display absolute values, normalised values, and print the colour of the card
                    sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                            abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "blue");
                } else {                                        // otherwise, light blue card
                    // Light blue card - Turn left 135 degrees
                    // display absolute values, normalised values, and print the colour of the card
                    sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                            abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "light blue");
                }
            }
        // Other colours
        } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {     // range for yellow card
            // Yellow card - Reverse 1 square and turn right 90 degrees
            // display absolute values, normalised values, and print the colour of the card
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "yellow");
            
        } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {    // range for pink card
            // Pink card - Reverse 1 square and turn left 90 degrees
            // display absolute values, normalised values, and print the colour of the card
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "pink");
            
        } else if ((rel.R<0.47) && (rel.G>0.295) && (rel.B>0.21)) {     // range for white card
            // White card - Finish (return home)
            // display absolute values, normalised values, and print the colour of the card
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "white");
            
        } else {                                                        // range for unknown (black) card
            // Unknown card - Return back to the starting position if final card cannot be found
            // display absolute values, normalised values, and print the colour of the card
            sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f     Colour: %s\n\r",\
                    abs.R, abs.G, abs.B, abs.C, rel.R, rel.G, rel.B, "unknown");
        }
        sendStringSerial4(buf);        // send to string to serial communication
        __delay_ms(500);               // delay so can read lines on monitor
    }
}

/********************************************
 * colourcards_normaliseRGBC
 * Function used to normalise the RGBC values
 ********************************************/
void colourcards_normaliseRGBC(RGBC_val *abs, RGB_rel *rel) {
    unsigned int R = abs->R; // initialise value for R from absolute value pointer 
    unsigned int G = abs->G; // initialise value for G from absolute value pointer
    unsigned int B = abs->B; // initialise value for B from absolute value pointer
    unsigned int C = abs->C; // initialise value for C from absolute value pointer
    
    rel->R = (float)R/(float)C; // normalise R value by finding percentage and store with pointer to rel
    rel->G = (float)G/(float)C; // normalise R value by finding percentage and store with pointer to rel
    rel->B = (float)B/(float)C; // normalise R value by finding percentage and store with pointer to rel
}
