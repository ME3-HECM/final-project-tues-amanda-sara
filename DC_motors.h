#ifndef _DC_MOTORS_H // This is a guard condition so that contents of this file are not included more than once
#define _DC_MOTORS_H // Beginning of _DC_MOTORS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/********************
 * DC motor structure
 ********************/
typedef struct {                 // Define a structure
    char power;                  // Motor power, out of 100
    char direction;              // Motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; // PWM duty high byte address
    unsigned char *dir_LAT;      // LAT for direction pin
    char dir_pin;                // Pin number that controls direction on LAT
    int PWMperiod;               // Base period of PWM cycle
} DC_motor;                      // Name this structure DC_motor

/******************
 * Global variables
 ******************/
extern volatile char turnleft_delay;           // Adjustable value to calibrate motor left turn (refer main.h)
extern volatile char turnright_delay;          // Adjustable value to calibrate motor right turn (refer main.h)
extern volatile unsigned char returnhome_flag; // Toggled when buggy has found the final white card or in exceptions (refer main.h)

/*********************
 * Function prototypes
 *********************/
void DCmotors_init(unsigned char PWMperiod);                  // Function used to initialise T2 and PWM for DC motor control
void DCmotors_setPWM(DC_motor *m);                            // Function used to set PWM output from the values in the motor structure
void checkbatterylevel(void);                                 // Function used to check battery level on the buggy
void forward(DC_motor *mL, DC_motor *mR);                     // Function used to move the buggy forward
void reverse(DC_motor *mL, DC_motor *mR);                     // Function used to move the buggy backwards
void clearance(DC_motor *mL, DC_motor *mR);                   // Function used to create space clearance for the buggy to turn
void stop(DC_motor *mL, DC_motor *mR);                        // Function used to gradually stop the buggy
void left(DC_motor *mL, DC_motor *mR, unsigned int deg);      // Function used to rotate the buggy wheels leftwards
void right(DC_motor *mL, DC_motor *mR, unsigned int deg);     // Function used to rotate the buggy wheels rightwards
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg);  // Function used to turn the buggy to the left
void turnright(DC_motor *mL, DC_motor *mR, unsigned int deg); // Function used to turn the buggy to the right
void adjdelay(unsigned char mode);                            // Function used to calibrate the DC motors to adapt to different surfaces
void DCmotors_calibration(DC_motor *mL, DC_motor *mR);        // Function used to adjust the turning duration and hence the turning angle
void DCmotors_testing(DC_motor *mL, DC_motor *mR);            // Function used to test all the DC motor movements

#endif // End of _DC_MOTORS_H
