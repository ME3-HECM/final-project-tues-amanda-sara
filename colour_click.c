#include <xc.h>
#include "colour_click.h"
#include "buttons_n_LEDs.h"
#include "I2C.h"

#define AILTL 0x04
#define AILTH 0x05
#define AIHTL 0x06
#define AIHTH 0x07

volatile unsigned int clear_lower;
volatile unsigned int clear_upper;

/***************************************************************
 * colorclick_init
 * Function used to initialise the colour click module using I2C
 ***************************************************************/
void colourclick_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 colourclick_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	colourclick_writetoaddr(0x00, 0x03);

    //set integration time
	colourclick_writetoaddr(0x01, 0xD5);
    
    colourclickLEDs_init();
}

/*************************************************************
 * colorclick_writetoaddr
 * Function used to write to the colour click module
 * Address is the register within the colour click to write to
 * Value is the value that will be written to that address
 *************************************************************/
void colourclick_writetoaddr(char address, char value)
{
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

/**********************************************************
 * colorclick_readRed
 * Function used to read the red channel
 * Returns a 16 bit ADC value representing colour intensity
 **********************************************************/
unsigned int colourclick_readR(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*********************************************************
 * colorclick_readGreen
 * Function to read the green channel
 * Returns a 16 bit ADC value representing color intensity
 *********************************************************/
unsigned int colourclick_readG(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at GREEN low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Green LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Green MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/********************************************
 * colorclick_readBlue
 * Function to read the blue channel
 * Returns a 16 bit ADC value representing color intensity
 *********************************************************/
unsigned int colourclick_readB(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Blue LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*********************************************************
 * colorclick_readClear
 * Function to read the clear channel
 * Returns a 16 bit ADC value representing color intensity
 *********************************************************/
unsigned int colourclick_readC(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Blue LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/**********************
 * colorclick_readColor
 * Function used to
 **********************/
void colourclick_readRGBC(RGBC_val *tmpval)
{
    tmpval->R = colourclick_readR();
    tmpval->G = colourclick_readG();
    tmpval->B = colourclick_readB();
    tmpval->C = colourclick_readC();
}

/**********************
 * colorclick_readColor
 * Function used to
 **********************/
void colourclick_readRGBC2(RGBC_val *tmpval)
{
    colourclickLEDs_C(0);
    
    RED_LED = 1;
    tmpval->R = colourclick_readR();
    __delay_ms(1000);
    RED_LED = 0;
    __delay_ms(20);
    
    GREEN_LED = 1;
    tmpval->G = colourclick_readG();
    __delay_ms(1000);
    GREEN_LED = 0;
    __delay_ms(20);
    
    BLUE_LED = 1;
    tmpval->B = colourclick_readB();
    __delay_ms(1000);
    BLUE_LED = 0;
    __delay_ms(20);
    
    colourclickLEDs_C(1);
}

/******************
 * 
 ********************/
void colourclick_calibration(void) {
    RGBC_val initial;
    while(RF2_BUTTON);
    RD7_LED = 1;
    colourclick_readRGBC(&initial); //read initial light value
    __delay_ms(100);
    RD7_LED = 0;
    
    unsigned char i;
    RGBC_val current;
    for (i=0; i<10; i++) {
        while(RF2_BUTTON);
        RD7_LED = 1;
        colourclick_readRGBC(&current);
        if ((current.C<initial.C) && (current.C>interrupts_lower)) {
            interrupts_lower = current.C;
        } else if ((current.C>initial.C) && (current.C<interrupts_upper)) {
            interrupts_upper = current.C;
        }
        __delay_ms(100);
        RD7_LED = 0;
    }
}