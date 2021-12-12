#ifndef _BUTTONS_N_LEDS_H // This is a guard condition so that contents of this file are not included more than once
#define	_BUTTONS_N_LEDS_H // Beginning of _BUTTONS_N_LEDS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/******************
 * Clicker 2 buttons
 ******************/
#define RF2_BUTTON PORTFbits.RF2 // Polls the state of RF2 button
#define RF3_BUTTON PORTFbits.RF3 // Polls the state of RF3 button

/***************
 * Clicker 2 LEDs
 ***************/
#define RD7_LED LATDbits.LATD7 // Toggles the RD7 LED
#define RH3_LED LATHbits.LATH3 // Toggles the RH3 LED

/************
 * Buggy LEDs
 ************/
#define HEADLAMPS_LED LATHbits.LATH1 // Toggles the front white LEDs and rear red LEDs (at a reduced brightness)
#define MAINBEAM_LED LATDbits.LATD3  // Toggles the front white LEDs (at full brightness)
#define BRAKE_LED LATDbits.LATD4     // Toggles the rear red LEDs (at full brightness)
#define TURNLEFT_LED LATFbits.LATF0  // Toggles the left signal (but no hardware based brightness control)
#define TURNRIGHT_LED LATHbits.LATH0 // Toggles the right signal (but no hardware based brightness control)

/*******************
 * Colour click LEDs
 *******************/
#define RED_LED LATGbits.LATG1   // Toggles the red LED
#define GREEN_LED LATAbits.LATA4 // Toggles the green LED
#define BLUE_LED LATFbits.LATF7  // Toggles the blue LED

/*********************
 * Function prototypes
 *********************/
void clicker2buttons_init(void);
void clicker2LEDs_init(void);
void buggyLEDs_init(void);
void colourclickLEDs_init(void);
void colourclickLEDs_RGB(void);
void colourclickLEDs_C(unsigned char tog);

#endif // End of _BUTTONS_N_LEDS_H
