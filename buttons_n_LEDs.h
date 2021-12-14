#ifndef _BUTTONS_N_LEDS_H // This is a guard condition so that contents of this file are not included more than once
#define	_BUTTONS_N_LEDS_H // Beginning of _BUTTONS_N_LEDS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/**********************************
 * Clicker 2 board buttons and LEDs
 **********************************/
#define RF2_BUTTON PORTFbits.RF2 // Polls the state of RF2 button
#define RF3_BUTTON PORTFbits.RF3 // Polls the state of RF3 button
#define RD7_LED LATDbits.LATD7   // Toggles the RD7 LED
#define RH3_LED LATHbits.LATH3   // Toggles the RH3 LED

/**************************
 * Colour click module LEDs
 **************************/
#define RED_LED LATGbits.LATG1   // Toggles the red LED
#define GREEN_LED LATAbits.LATA4 // Toggles the green LED
#define BLUE_LED LATFbits.LATF7  // Toggles the blue LED

/************
 * Buggy LEDs
 ************/
#define HEADLAMPS_LED LATHbits.LATH1 // Toggles the front white LEDs and rear red LEDs (at a reduced brightness)
#define MAINBEAM_LED LATDbits.LATD3  // Toggles the front white LEDs (at full brightness)
#define BRAKE_LED LATDbits.LATD4     // Toggles the rear red LEDs (at full brightness)
#define TURNLEFT_LED LATFbits.LATF0  // Toggles the left signal (but no hardware based brightness control)
#define TURNRIGHT_LED LATHbits.LATH0 // Toggles the right signal (but no hardware based brightness control)

/*********************
 * Function prototypes
 *********************/
void clicker2buttons_init(void);           // Function used to initialise the buttons on the clicker 2 board
void clicker2LEDs_init(void);              // Function used to initialise the LEDs on the clicker 2 board
void colourclickLEDs_init(void);           // Function used to initialise the LEDs on the colour click module
void colourclickLEDs_RGB(void);            // Function used to cycle the red, green and blue LEDs on the colour click module
void colourclickLEDs_C(unsigned char tog); // Function used to toggle the clear light on the colour click module
void buggyLEDs_init(void);                 // Function used to initialise the LEDs on the buggy

#endif // End of _BUTTONS_N_LEDS_H
