#ifndef _MAIN_H // This is a guard condition so that contents of this file are not included more than once
#define	_MAIN_H // Beginning of _MAIN_H

/**********************************************************
 * Definitions
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/**********************************************************
 * File inclusions
 * See The C programming language, second edition, pp.88-89
 **********************************************************/
#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#include "ADC.h"            // Include header file to initilaise ADC module to check battery level of the buggy
#include "buttons_n_LEDs.h" // Include header file to initialise and use buttons and LEDs on the clicker 2 board, colour click module and buggy
#include "colour_cards.h"   // Include header file to identify colour cards and respond accordingly
#include "colour_click.h"   // Include header file to initialise and use the colour click module for colour recognition
#include "DC_motors.h"      // Include header file to initialise and operate the DC motors on the buggy
#include "I2C.h"            // Include header file to initialise and use I2C with the colour click module
#include "interrupts.h"     // Include header file to initialise and trigger interrupts when colour card is found / buggy is stuck in the maze for too long
#include "serial_comm.h"    // Include header file to initialise and use serial communication to provide on-screen feedback for testing purposes
#include "timers.h"         // Include header file to initialise and use timer to figure out whether the buggy has been stuck in the maze for too long

/***************************************************
 * CONFIG1L (configuration word 1) - oscillators
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.28-29
 ***************************************************/
#pragma config FEXTOSC = HS         // FEXTOSC external oscillator mode selection bits (HS (crystal oscillator) above 8 MHz; PFM (program memory) set to high power)
#pragma config RSTOSC = EXTOSC_4PLL // Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

/***********************************************************
 * CONFIG3L (configuration word 3) - windowed watchdog timer
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.32-33
 ***********************************************************/
#pragma config WDTE = OFF // WDT operating mode bits (WDT enabled regardless of sleep)

/******************
 * Global variables
 ******************/
volatile int turnleft_calangle;              // Angle turned left by motor during calibration
volatile int turnright_calangle;             // Angle turned right by motor during calibration
volatile unsigned int interrupts_lowerbound; // Lower clear threshold value to trigger interrupts when encounter colour cards
volatile unsigned int interrupts_upperbound; // Upper clear threshold value to trigger interrupts when encounter colour cards
volatile unsigned char colourcard_flag;      // Toggled when buggy encounters a colour card
volatile unsigned char unknowncard_flag;     // Incremented each time the buggy fails to identify a colour card
volatile unsigned char returnhome_flag;      // Toggled when buggy has found the final white card or in exceptions
volatile unsigned char overtime_flag;        // Toggled when buggy has been stuck in the maze for too long

#endif // End of _MAIN_H
