#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#include "colour_click.h"   // Include corresponding header file
#include "buttons_n_LEDs.h" // Include header file to use buttons and LEDs on the clicker 2 board
#include "I2C.h"            // Include header file to use I2C for programming the colour click module
#include "serial_comm.h"    // Include header file to use serial communication module to output results on-screen for testing purposes

/***************************************************************
 * colourclick_init
 * Function used to initialise the colour click module using I2C
 ***************************************************************/
void colourclick_init(void) {   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();                  //Initialise I2C Master

     //set device PON
	 colourclick_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
    __delay_ms(3);                        //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	colourclick_writetoaddr(0x00, 0x03); // write to enable register and turn on ADC bit

    //set integration time
	colourclick_writetoaddr(0x01, 0xD5); // write to timing register and set integration time to 101ms
    
    colourclickLEDs_init();              // initialise R,G,B LEDs 
}

/***************************************************
 * colourclick_writetoaddr
 * Function used to write to the colour click module
 ***************************************************/
void colourclick_writetoaddr(char address, char value) {
    I2C_2_Master_Start();               //Start condition
    I2C_2_Master_Write(0x52 | 0x00);    //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address); //command + register address within the colour click to write to
    I2C_2_Master_Write(value);          // The value that will be written to that address
    I2C_2_Master_Stop();                //Stop condition
}

/***************************************************************************************************
 * colourclick_readR
 * Function used to read the red channel and return a 16 bit ADC value representing colour intensity
 ***************************************************************************************************/
unsigned int colourclick_readR(void) {
	unsigned int tmp;                                  // Initialise variable to store 16 bit red colour intensity value
	I2C_2_Master_Start();                              // Start condition
	I2C_2_Master_Write(0x52 | 0x00);                   // 7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);                   // Command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			               // Start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);                   // 7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			               // Read the Red LSB
	tmp=tmp | (unsigned int)(I2C_2_Master_Read(0)<<8); // Read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                               // Stop condition
	return tmp;                                        // Return 16 bit red colour intensity value
}

/***************************************************************************************************
 * colourclick_readG
 * Function used to read the green channel and return a 16 bit ADC value representing colour intensity
 ***************************************************************************************************/
unsigned int colourclick_readG(void) {
	unsigned int tmp;                                  // Initialise variable to store 16 bit green colour intensity value
	I2C_2_Master_Start();                              // Start condition
	I2C_2_Master_Write(0x52 | 0x00);                   // 7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);                   // command (auto-increment protocol transaction) + start at GREEN low register
	I2C_2_Master_RepStart();			               // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);                   // 7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			               // read the Green LSB
	tmp=tmp | (unsigned int)(I2C_2_Master_Read(0)<<8); // read the Green MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                               // Stop condition
	return tmp;                                        // Return 16 bit green colour intensity value
}

/***************************************************************************************************
 * colourclick_readB
 * Function used to read the blue channel and return a 16 bit ADC value representing colour intensity
 ***************************************************************************************************/
unsigned int colourclick_readB(void) {
	unsigned int tmp;                                  // Initialise variable to store 16 bit blue colour intensity value
	I2C_2_Master_Start();                              // Start condition
	I2C_2_Master_Write(0x52 | 0x00);                   // 7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);                   // Command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			               // Start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);                   // 7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			               // Read the Blue LSB
	tmp=tmp | (unsigned int)(I2C_2_Master_Read(0)<<8); // Read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                               // Stop condition
	return tmp;                                        // Return 16 bit blue colour intensity value
}

/*****************************************************************************************************
 * colourclick_readC
 * Function used to read the clear channel and return a 16 bit ADC value representing colour intensity
 *****************************************************************************************************/
unsigned int colourclick_readC(void) {
	unsigned int tmp;                                  // Initialise variable to store 16 bit clear colour intensity value
	I2C_2_Master_Start();                              // Start condition
	I2C_2_Master_Write(0x52 | 0x00);                   // 7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);                   // Command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			               // Start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);                   // 7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			               // Read the Blue LSB
	tmp=tmp | (unsigned int)(I2C_2_Master_Read(0)<<8); // Read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                               // Stop condition
	return tmp;                                        // Return 16 bit clear colour intensity value
}

/*********************************************************
 * colourclick_readRGBC
 * Function used to read the RGBC values using clear light
 *********************************************************/
void colourclick_readRGBC(RGBC_val *tmpval) {
    // read each value and store in relevant address
    tmpval->R = colourclick_readR(); //
    tmpval->G = colourclick_readG(); //
    tmpval->B = colourclick_readB(); //
    tmpval->C = colourclick_readC(); //
}

/*************************************************************************
 * colourclick_readRGBC2
 * Function used to read the RGBC values using the red, green or blue LEDs
 *************************************************************************/
void colourclick_readRGBC2(RGBC_val *tmpval, unsigned char mode) {
    colourclickLEDs_C(0);             // Switch off all colour click LEDs
    __delay_ms(100);                  //
    
    if (mode==1) {                    // red LED mode 
        RED_LED = 1;                  // turn on red LED
        __delay_ms(1000);             // wait until on fully to read
        colourclick_readRGBC(tmpval); // read values with red LED on
        __delay_ms(1000);             //
        RED_LED = 0;                  // turn off red LED
        __delay_ms(20);               //
        
    } else if (mode==2) {             // green LED mode
        GREEN_LED = 1;                // turn on green LED
        __delay_ms(1000);             // wait until on fully to read
        colourclick_readRGBC(tmpval); // read values with green LED on
        __delay_ms(1000);             // 
        GREEN_LED = 0;                // turn off green LED
        __delay_ms(20);               //
        
    } else if (mode==3) {             // blue LED mode
        BLUE_LED = 1;                 // turn on blue LED
        __delay_ms(1000);             // wait until on fully to read
        colourclick_readRGBC(tmpval); // read values with blue LED on
        __delay_ms(1000);             //
        BLUE_LED = 0;                 // turn off blue LED
        __delay_ms(20);               //
    }
    
    colourclickLEDs_C(1);             // Switch back on colour click clear light
    __delay_ms(100);                  //
}

/****************************************************
 * colourclick_calibration
 * Function used to calibrate the colour click module
 ****************************************************/
void colourclick_calibration(void) {
    RGBC_val initial;
    while(RF2_BUTTON && RF3_BUTTON); // wait for button press
    if (!RF2_BUTTON) {RD7_LED=1;}    // 
    if (!RF3_BUTTON) {RH3_LED=1;}    //
    MAINBEAM_LED = 1;                // turn on buggy main LEDs
    colourclickLEDs_C(1);            // turn on clicker white LED
    __delay_ms(1000);                // wait to turn on fully
    
    colourclick_readRGBC(&initial);  // read initial ambient light value
    
    __delay_ms(1000);                //   
    MAINBEAM_LED = 0;                // turn off LEDs
    colourclickLEDs_C(0);            
    RD7_LED = 0;                     
    RH3_LED = 0;                     
    
    interrupts_lowerbound = initial.C - 150; // set lower interrupt threshold with initial clear value
    interrupts_upperbound = initial.C + 100; // set upper interrupt threshold with initial clear value 
}

/***********************************************
 * colourclick_testing
 * Function used to test the colour click module
 ***********************************************/
void colourclick_testing(RGBC_val *initval, RGBC_val *tmpval) {
    unsigned int ambient = initval->C; //
    unsigned int R = tmpval->R;        //
    unsigned int G = tmpval->G;        //
    unsigned int B = tmpval->B;        //
    unsigned int C = tmpval->C;        //
    
    if ((C>interrupts_lowerbound) && (C<ambient)) {interrupts_lowerbound = C;}    //
    else if ((C<interrupts_upperbound) && (C>ambient)) {interrupts_upperbound=C;} //
    
    char buf[100];                                                                //
    sprintf(buf,"RGBC: %i %i %i %i     Threshold: %i %i %i\n\r",\
            R, G, B, C, interrupts_lowerbound, ambient, interrupts_upperbound);   //
    sendStringSerial4(buf);                                                       //
    __delay_ms(500);                                                              //
}
