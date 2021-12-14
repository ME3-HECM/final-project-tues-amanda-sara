# Final project

---

# Overview

After a short calibration routine, our buggy autonomously navigates through a "mine" to find a target, and heads back home after it does so. The buggy navigates through the mine by using a clicker colour sensor to detect a series of coloured cards. Each colour has an action attached to it, and by following the instructions, the buggy can navigate to the target signified by a white card 

| Colour | Instruction |
| --- | --- |
| Red | Turn Right 90 |
| Green | Turn Left 90 |
| Blue | Turn 180 |
| Yellow | Reverse 1 square and turn right 90 |
| Pink | Reverse 1 square and turn left 90 |
| Orange | Turn Right 135 |
| Light blue | Turn Left 135 |
| White | Return home |

The buggy detects there is a card in front of it by using a clear interrupt on the colour click. When the clear channel reads outside of a specified threshold, the interrupt is triggered and the buggy stops. The buggy then goes through it's routine to identify what colour card is in front of it. The red, green and blue channels are converted into a ratio from which each colour can be identified. 

---

# User instructions

To account for different ambient light conditions and surface conditions, the buggy has a calibration routine to ensure that the colour sensor and turns are accurate. After this the buggy should run autonomously. Tires were removed for more accurate turning and a cover was made around the sensor to somewhat block out the ambient light. 

## Calibration

### DC motor calibration routine

First the motor is calibrated so that the turns are accurate to the specified angle. To begin the calibration, either button RF2/RF3 can be pressed. The buggy will then execute a 360º left turn and come to stop. If the turn was off in one direction, pressing and holding the button in the direction that the buggy needs to turn to get to 360º will change the value for the turn i.e. if the buggy needs to turn more left to get to 360º, press and hold the left button. The light will flash for very increase in the specified direction. If no change is needed, a short press of either button will proceed to the right turn calibration, which has the same procedure. 

Once this has been done, the buggy will turn in left 360º, then right 360º to show the result of the calibration. If the user is happy with the result, RF2 can be pressed and the buggy will move onto the colour calibration. If not, RF3 can be pressed and the calibration will perform again. 

### Colour calibration

Once the motor calibration had been completed, the buggy is placed in its starting position. Pressing either RF2/RF3 will start the colour calibration in which the lights will flash and an initial value of the ambient light will be taken. This is used to set the thresholds for the colour interrupt so the buggy knows to stop when there is a card in front of it. 

Once both calibration routines have been completed, either button can be pressed to start the mine navigation.

---

# Code structure

## main.c

The main file first initialises the functions for the buggy, then initialises the variables for the left and right motors in the specified structure (see DC_motors.h for more information). The main then calls the calibration functions then moves to the main while loop. 

The first thing inside the main loop is a function to prevent false triggers of the clear channel interrupt, and clears any flags from the false trigger before the car starts moving. It then checks to see whether the clear channel interrupt has been triggered. If so, the car stops, reverses slightly, runs the colour sensor routine (see colour_cards.c for more information), resets the threshold values for the interrupt for the new ambient light conditions by taking a colour reading, then finally clears the interrupt variable 'colourcard_flag'. 

```c
} else if (start>0 && colourcard_flag==1) {
            stop(&motorL, &motorR);
            RD7_LED = 1;
            RH3_LED = 1;
            reverse(&motorL, &motorR);
            __delay_ms(100);
            stop(&motorL, &motorR);
            RD7_LED = 0;
            RH3_LED = 0;
            __delay_ms(1000);
            
            colourcards_readRGBC(&current, &motorL, &motorR);
            
            __delay_ms(1000);
            colourclick_readRGBC(&current);
            interrupts_lowerbound = current.C - 150;
            interrupts_upperbound = current.C + 100;
            
            colourcard_flag = 0;
```

If the interrupt has not been triggered, then the buggy checks whether it has been told to retrace it's step with the 'returnhome_flag'. If so, the return routine is run. In all other cases, which is most of the time it is running, the buggy moves forwards.

## colour_click.c/h

This file contains the function to initialise the colour clicker, write to it's different addresses and read the values it takes from the RGBC channels. The RGBC values are stored as unsigned int in a specified RGBC_val structure (see  below) for easier retrieval.

```c
typedef struct {             // Define a structure
    unsigned int R, G, B, C; // Containing the RGBC values read by the colour click
} RGBC_val;                  // This structure is named RGB_val
```

All the RGBC read functions are combined into one colourclick_readRGBC(RGBC_val *tmpval) function, which stores all the values with pointers into given RGBC_val type variable (see below).

```c
 void colourclick_readRGBC(RGBC_val *tmpval) {
    tmpval->R = colourclick_readR();
    tmpval->G = colourclick_readG();
    tmpval->B = colourclick_readB();
    tmpval->C = colourclick_readC();
}
```

A second RGBC read function is used to take readings when specific lights are shone rather than the white light. This is used to differentiate between 'difficult' colour e.g. orange and red (see colour_card.c/h for more information). The above function is called to read and store the values each time, and which LED is shone can be set by setting the 'mode'. 

```c
void colourclick_readRGBC2(RGBC_val *tmpval, unsigned char mode) {
    colourclickLEDs_C(0);
    __delay_ms(100);
    
    if (mode==1) {
        RED_LED = 1;
        __delay_ms(1000);
        colourclick_readRGBC(tmpval);
        __delay_ms(1000);
        RED_LED = 0;
        __delay_ms(20);
    } else if (mode==2) {
        GREEN_LED = 1;
        __delay_ms(1000);
        colourclick_readRGBC(tmpval);
        __delay_ms(1000);
        GREEN_LED = 0;
        __delay_ms(20);
    } else if (mode==3) {
        BLUE_LED = 1;
        __delay_ms(1000);
        colourclick_readRGBC(tmpval);
        __delay_ms(1000);
        BLUE_LED = 0;
        __delay_ms(20);
    }
    
    colourclickLEDs_C(1);
    __delay_ms(100);
}
```

This file also contains the calibration for the colour values. An initial light reading is taken, which is used to set the thresholds for the interrupts (see interrupts.c/h for more information). 

## colour_cards.c/h

This file contains the function to identify the card colour and execute the action. Using the white LED, the light reading is taken and the RGB values are found as a percentages. Using this alone, most colours can be identified. There are however two groups of colours that are similar and further readings need to be taken. 

To differentiate between the red/orange cards, the blue LED is flashed and relative values are taken. For the blue/green/light blue cards, the red and green LEDs are flashed and values taken. 

Once the colour has been identified, the buggy reverses so it has clearance to turn, and the relevant action is executed (called through the instruction function, see DC_motor.c/h for details).

If the colour is white, the return home flag is tripped and the return routine begins. If the sensor cannot figure out what card is in front of it, or if it has hit a wall, the buggy will move forwards slightly and try three times to identify the card. If after three attempts the card cannot be identifies, the return home routine is also triggered. 

```c
void colourcards_readRGBC(RGBC_val *abs, DC_motor *mL, DC_motor *mR) {
    // Switch off interrupts (to avoid unwanted interrupts while identifying cards)
    PIE0bits.INT1IE = 0;
    
    // Current values at 5 cm distance
    RGB_rel rel;
    colourclick_readRGBC(abs);
    colourcards_normaliseRGBC(abs, &rel);
    
    // Threshold values at 5 cm distance
    // Red/orange
    if ((rel.R>0.54) && (rel.G<0.245) && (rel.B<0.18)) {
        colourclick_readRGBC2(abs, 3); // Blue LED
        colourcards_normaliseRGBC(abs, &rel);
        if (rel.G<0.185) {
            // Red card - Turn right 90 degrees
            instructions(mL, mR, 1);
        } else {
            // Orange card - Turn right 135 degrees
            instructions(mL, mR, 6);
        }

    // Green/blue/light blue
    } else if ((rel.R<0.44) && (rel.G>0.30) && (rel.B>0.195)) {
        colourclick_readRGBC2(abs, 1); // Red LED
        colourcards_normaliseRGBC(abs, &rel);
        if (rel.B<0.125) {
            // Green card - Turn left 90 degrees
            instructions(mL, mR, 2);
        } else {
            colourclick_readRGBC2(abs, 2); // Green LED
            colourcards_normaliseRGBC(abs, &rel);
            if (rel.R<0.115) {
                // Blue card - Turn 180 degrees
                instructions(mL, mR, 3);
            } else {
                // Light blue card - Turn left 135 degrees
                instructions(mL, mR, 7);
            }
        }
    // Other colours
    } else if ((rel.R>0.49) && (rel.G>0.285) && (rel.B>0.18)) {
        // Yellow card - Reverse 1 square and turn right 90 degrees
        instructions(mL, mR, 4);

    } else if ((rel.R>0.49) && (rel.G<0.275) && (rel.B>0.195)) {
        // Pink card - Reverse 1 square and turn left 90 degrees
        instructions(mL, mR, 5);

    } else if ((rel.R<0.47) && (rel.G>0.295) && (rel.B>0.21)) {
        // White card - Finish (return home)
        unknowncard_flag = 0;
        returnhome_flag = 1;

    } else {
        // Unknown card - Return back to the starting position if final card cannot be found
        __delay_ms(1000);
        forward(mL, mR);
        __delay_ms(10);
        stop(mL, mR);
        colourclick_readRGBC(abs);
        if ((abs->C < interrupts_lowerbound) || (abs->C > interrupts_upperbound)) {
            if (unknowncard_flag<3) {
                unknowncard_flag++;
                PIR0bits.INT1IF = 1;
            } else {
                RH3_LED = 1;
                returnhome_flag = 1;
            }
        } else {unknowncard_flag = 0;}
    }
    
    // Switch on interrupts again (to prepare for the next card)
    PIE0bits.INT1IE = 1;
}
```

The values for each card were found by testing with the serial ports and the colourcards_testingRGBC2() function. 

## DC_motors.c/h

This file contains initialisation for the motor and the basic functions for moving the buggy. The left and right functions takes a degree argument to control how far to turn. The degree given is converted to a delay using a linear calculation. The longer the delay, the longer that the turn function is on for, and therefore the more the buggy turns (see below).

```c
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg) {
    // Calculations
    double delay = ((deg*2.332)+31.506) * 360/turnleft_calangle;
    
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    mL->direction = 0; // left wheels go backward
    mR->direction = 1; // right wheels go forward

    // make both motors accelerate
    TURNLEFT_LED = 1;
    while((mL->power < 70) || (mR->power < 70)){
        // gradually turn left
        if (mL->power < 70) {mL->power += 10;}
        if (mR->power < 70) {mR->power += 10;}

        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
    
    unsigned int i;
    for (i=0; i<delay; i++) {__delay_ms(1);}
    TURNLEFT_LED = 0;
} 
```

This file also has the motor calibration function. The calibration works by adjusting the amount of the time that the turn function is on for by changing a constant in the delay calculations. In the adjust delay function, if for example the right button is pressed, the constant will change in favour of the right direction. 

```c
if (mode==1){ // Turn right
            if (!RF2_BUTTON && RF3_BUTTON) {
                RD7_LED = 1;
                turnright_calangle -= 5;
                __delay_ms(800);
                RD7_LED = 0;               
            } else if (!RF3_BUTTON && RF2_BUTTON) {
                RH3_LED = 1;
                turnright_calangle += 5;
                __delay_ms(800);
                RH3_LED = 0;
            }
```

The instructions for each card is also specified here in the instructions() function, in which each colour is given unique number which is called and the action is executed by the function. The order of the numbers are remembered and stored so the reverse can be executed when the buggy needs to 'return home'. The time between instructions is given by the duration recorded in the forwards direction using the timer interrupts (see interrupts.c/h for more information).

## interrupts.c/h

The buggy uses the clear channel interrupt on the colour clicker to sense when it is near to a card to stop and run the colour card routine. The external interrupt and the peripheral pin select is initialised here for the microbus 2 pins.  

A function was written to initialise the interrupt on the colour clicker (see below). The first write enables the interrupt, the second sets the persistence register to trigger when there have been 5 readings outside the specified range, and the remaining writes set the range values outside of which the interrupt will trigger. These are shifted values taken from the ambient light reading so that after each turn the interrupt will be set for the new light conditions. This accounts for changes in overhead lighting orientation (i.e. casting shadows) and changes in light levels (i.e. the sun). 

```c
void interrupts_colourclick(void){
    colourclick_writetoaddr(0x00, 0b10011);
    __delay_ms(3);
    colourclick_writetoaddr(0x0C, 0b0100);
    colourclick_writetoaddr(0x04, (interrupts_lowerbound & 0x00FF));
    colourclick_writetoaddr(0x05, (interrupts_lowerbound >> 8));
    colourclick_writetoaddr(0x06, (interrupts_upperbound & 0x00FF));
    colourclick_writetoaddr(0x07, (interrupts_upperbound >> 8));
}
```

Another function was made to force clear the interrupt flag on the clicker, and reinitialise the interrupt after every clear. 

```c
void interrupts_clear(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);    //command + register address  
    I2C_2_Master_Stop();
    
    interrupts_colourclick();
}
```

The clear channel interrupt was set as the high priority interrupt, and the low priority interrupt has a timer0 overflow to measure the duration between instruction for the return home function.