#include <xc.h>
#include <stdio.h>
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
    float R_rel = (float)tmpval->R / (float)tmpval->C;
    float G_rel = (float)tmpval->G / (float)tmpval->C;
    float B_rel = (float)tmpval->B / (float)tmpval->C;
    
    // Threshold values at 5 cm distance
    if ((R_rel>0.54) && (G_rel<0.245) && (B_rel<0.18)) {
        // Red card - Turn right 90 degrees
        turnright(mL, mR, 90);
        unknowncard_flag = 0;
        
    } else if ((R_rel<0.435) && (G_rel>0.31) && (B_rel>0.195)) {
        // Green card - Turn left 90 degrees
        turnleft(mL, mR, 90);
        unknowncard_flag = 0;
        
    } else if ((R_rel<0.43) && (G_rel>0.30) && (B_rel>0.21)) {
        // Blue card - Turn 180 degrees
        turnright(mL, mR, 180);
        unknowncard_flag = 0;
        
    } else if ((R_rel>0.49) && (G_rel>0.285) && (B_rel>0.18)) {
        // Yellow card - Reverse 1 square and turn right 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        turnright(mL, mR, 90);
        unknowncard_flag = 0;
        
    } else if ((R_rel>0.49) && (G_rel<0.275) && (B_rel>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        turnleft(mL, mR, 90);
        unknowncard_flag = 0;
        
    } else if ((R_rel>0.54) && (G_rel<0.24) && (B_rel<0.18)) {
        // Orange card - Turn right 135 degrees
        turnright(mL, mR, 135);
        unknowncard_flag = 0;
        
    } else if ((R_rel<0.44) && (G_rel>0.305) && (B_rel>0.21)) {
        // Light blue card - Turn left 135 degrees
        turnleft(mL, mR, 135);
        unknowncard_flag = 0;
        
    } else if ((R_rel<0.46) && (G_rel>0.295) && (B_rel>0.21)) {
        // White card - Finish (return home)
        turnright(mL, mR, 180);
        unknowncard_flag = 0;
        returnhome_flag = 1;
        
    } else {
        // Exceptions (return back to the starting position if final card cannot be found)
        __delay_ms(1000);
        colourclick_readRGBC(tmpval);
        if ((tmpval->C < interrupts_lower) || (tmpval->C > interrupts_upper)) {
            if (unknowncard_flag<3) {
                PIR0bits.INT1IF = 1;
                unknowncard_flag++;
            } else {
                RH3_LED = 1;
                turnright(mL, mR, 180);
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
 * 
 ************************/
void colourcards_testing(void)
{
    INTCONbits.GIE = 0;
    
    RGBC_val current;
    colourclick_readRGBC(&current); //read current light value
    char buf[100];
    float R_rel = (float)current.R / (float)current.C;
    float G_rel = (float)current.G / (float)current.C;
    float B_rel = (float)current.B / (float)current.C;
    float C_rel = (float)current.C / (float)current.C;
    sprintf(buf,"RGBC: %i %i %i %i     RGBC_rel: %.3f %.3f %.3f %.3f\n\r",\
            current.R, current.G, current.B, current.C, R_rel, G_rel, B_rel, C_rel);
    sendStringSerial4(buf);
    __delay_ms(500);
    
    INTCONbits.GIE = 1;
}
