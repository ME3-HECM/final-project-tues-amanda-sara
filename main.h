#ifndef _MAIN_H // This is a guard condition so that contents of this file are not included more than once
#define	_MAIN_H // Beginning of _MAIN_H

/**********************************************************
 * Definitions
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/***********************************************************
 * File inclusions
 * See The C programming language, second edition, pp.88-89
 ***********************************************************/
#include <xc.h>    // Include processor file
#include <stdio.h> // Include standard input output library
#include "ADC.h"
#include "buttons_n_LEDs.h"
#include "colour_cards.h"
#include "colour_click.h"
#include "DC_motors.h"
#include "I2C.h"
#include "interrupts.h"
#include "serial_comm.h"

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
volatile unsigned int interrupts_lower; // Lower threshold value to trigger interrupts
volatile unsigned int interrupts_upper; // Upper threshold value to trigger interrupts
volatile unsigned char DCmotors_lower;
volatile unsigned char DCmotors_upper;
volatile unsigned char colourcard_flag;
volatile unsigned char unknowncard_flag;
volatile unsigned char returnhome_flag;

#endif // End of _MAIN_H
