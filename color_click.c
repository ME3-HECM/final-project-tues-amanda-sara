#include <xc.h>
#include "color_click.h"
#include "i2c.h"

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
void colorclick_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 colorclick_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	colorclick_writetoaddr(0x00, 0x03);

    //set integration time
	colorclick_writetoaddr(0x01, 0xD5);
    
    //set TRIS values
    TRISGbits.TRISG1 = 0; //red LED
    TRISAbits.TRISA4 = 0; //green LED
    TRISFbits.TRISF7 = 0; //blue LED
    
    //set LAT values
    colorclick_toggleClearLED(1);
}

/**************************
 * colorclick_cyclingRGBLED
 * Function used to
 **************************/
void colorclick_cyclingRGBLED(void)
{
    redLED = 1;
    __delay_ms(50);
    redLED = 0;
    __delay_ms(20);
    
    greenLED = 1;
    __delay_ms(50);
    greenLED = 0;
    __delay_ms(20);
    
    blueLED = 1;
    __delay_ms(50);
    blueLED = 0;
    __delay_ms(20);
}

/***************************
 * colorclick_toggleClearLED
 * Function used to
 ***************************/
void colorclick_toggleClearLED(unsigned char toggle)
{
    redLED = toggle;
    blueLED = toggle;
    greenLED = toggle;
}

/*************************************************************
 * colorclick_writetoaddr
 * Function used to write to the colour click module
 * Address is the register within the colour click to write to
 * Value is the value that will be written to that address
 *************************************************************/
void colorclick_writetoaddr(char address, char value)
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
unsigned int colorclick_readRed(void)
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
unsigned int colorclick_readGreen(void)
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
unsigned int colorclick_readBlue(void)
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
unsigned int colorclick_readClear(void)
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
RGB_val colorclick_readColour(RGB_val current)
{
    current.R = colorclick_readRed();
    current.G = colorclick_readGreen();
    current.B = colorclick_readBlue();
    current.C = colorclick_readClear();

    return current;
}


void colorclick_int_clear(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);    //command + register address  
    I2C_2_Master_Stop(); 
    colorclick_int_init();
}

void colorclick_int_init(void){
    colorclick_writetoaddr(0x00, 0b10011);
    __delay_ms(3);
    colorclick_writetoaddr(0x0C, 0b0100);
    colorclick_writetoaddr(AILTL, (clear_lower&&0b11111111));
    colorclick_writetoaddr(AILTH, (clear_lower>>8));
    colorclick_writetoaddr(AIHTL, (clear_upper&&0b11111111));
    colorclick_writetoaddr(AIHTH, (clear_upper>>8));
    
}
