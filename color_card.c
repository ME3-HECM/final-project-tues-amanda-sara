#include <xc.h>
#include "color_card.h"
#include "color_click.h"
#include "dc_motor.h"

float read_card_RGB(RGB_val current, DC_motor *mL, DC_motor *mR) {
    // At 5 cm distance
    float R_rel = (float)current.R / (float)current.C;
    float G_rel = (float)current.G / (float)current.C;
    float B_rel = (float)current.B / (float)current.C;
    
    if ((R_rel>0.54) && (G_rel<0.245) && (B_rel<0.18)) {
        // Red card - Turn right 90 degrees
        turnRight(mL, mR, 90);
        stop(mL, mR);
        
    } else if ((R_rel<0.435) && (G_rel>0.31) && (B_rel>0.195)) {
        // Green card - Turn left 90 degrees
        turnLeft(mL, mR, 90);
        stop(mL, mR);
        
    } else if ((R_rel<0.43) && (G_rel>0.30) && (B_rel>0.21)) {
        // Blue card - Turn 180 degrees
        turnRight(mL, mR, 180);
        stop(mL, mR);
        
    } else if ((R_rel>0.49) && (G_rel>0.285) && (B_rel>0.18)) {
        // Yellow card - Reverse 1 square and turn right 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        turnRight(mL, mR, 90);
        stop(mL, mR);
        
    } else if ((R_rel>0.49) && (G_rel<0.275) && (B_rel>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        reverse(mL, mR);
        stop(mL, mR);
        turnLeft(mL, mR, 90);
        stop(mL, mR);
        
    } else if ((R_rel>0.54) && (G_rel<0.24) && (B_rel<0.18)) {
        // Orange card - Turn right 135 degrees
        turnRight(mL, mR, 135);
        stop(mL, mR);
        
    } else if ((R_rel<0.44) && (G_rel>0.305) && (B_rel>0.21)) {
        // Light blue card - Turn left 135 degrees
        turnLeft(mL, mR, 135);
        stop(mL, mR);
        
    } else if ((R_rel<0.46) && (G_rel>0.295) && (B_rel>0.21)) {
        // White card - Finish (return home)
        turnRight(mL, mR, 180);
        stop(mL, mR);
        returnhome_flag = 1;
        
    } else {
        // Exceptions (return back to the starting position if final card cannot be found)
        turnRight(mL, mR, 180);
        stop(mL, mR);
        returnhome_flag = 1;
    }
    
    return R_rel;
}



void read_card_HSV(RGB_val current, DC_motor *mL, DC_motor *mR) {
    
}
