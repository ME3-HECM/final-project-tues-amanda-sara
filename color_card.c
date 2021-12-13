#include <xc.h>
#include "color_card.h"
#include "color_click.h"
#include "dc_motor.h"

void read_card(RGB_val initial, RGB_val current, DC_motor *mL, DC_motor *mR) {
    // At 5 cm distance
    unsigned int R_rel = current.R/current.C;
    unsigned int G_rel = current.G/current.C;
    unsigned int B_rel = current.B/current.C;
    
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
        turnRight(mL, mR, 90);
        stop(mL, mR);
        
    } else if ((R_rel>0.49) && (G_rel<0.275) && (B_rel>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        reverse(mL, mR);
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
        
    } else {
        // Exceptions (return back to the starting position if final card cannot be found)
        returnhome_flag = 1;
        turnRight(mL, mR, 180);
        stop(mL, mR);
    }
}

unsigned char stop_check(RGB_val current) {
    // At 15 cm distance
    float R_rel = current.R/current.C;
    float G_rel = current.G/current.C;
    float B_rel = current.B/current.C;
    float stop_pls = 0;
    
    if ((R_rel>0.474) && (G_rel<0.272) && (B_rel<0.193)) {
        // Red card - Turn right 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.453) && (G_rel>0.284) && (B_rel>0.193)) {
        // Green card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.452) && (G_rel>0.282) && (B_rel>0.196)) {
        // Blue card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.466) && (G_rel>0.281) && (B_rel>0.190)) {
        // Yellow card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.474) && (G_rel>0.272) && (B_rel>0.189)) {
        // Orange card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.466) && (G_rel>0.277) && (B_rel>0.194)) {
        // Pink card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.453) && (G_rel>0.284) && (B_rel>0.197)) {
        // Light blue card - Turn left 90 degrees
        stop_pls = 1;
        
    } else if ((R_rel<0.459) && (G_rel>0.283) && (B_rel>0.196)) {
        // white card - Turn left 90 degrees
        stop_pls = 1;
        
    } else {
        // Exceptions (return back to the starting position if final card cannot be found)
        stop_pls = 0;
    }
    return stop_pls = 0;
}