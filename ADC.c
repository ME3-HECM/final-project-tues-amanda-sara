#include <xc.h>
#include "ADC.h"

/***************************************************************************
 * ADC_init
 * Function used to initialise ADC module and set it up to sample on pin RF6 (BAT-VSENSE)
 ***************************************************************************/
void ADC_init(void) {
    // Initialise pin RF7
    TRISFbits.TRISF6=1;         // Select pin RF6 as input
    ANSELFbits.ANSELF6=1;       // Ensure analogue circuitry is active (it is by default)

    // Initialise the ADC module (check section 32 of the datasheet for more details)
    ADREFbits.ADNREF = 0;       // Use Vss (0V) as negative reference
    ADREFbits.ADPREF = 0b00;    // Use Vdd (3.3V) as positive reference
    ADPCH=0b101110;             // Select channel RF6/ANF6 for ADC (refer pg 631 of datasheet)
    ADCON0bits.ADFM = 0;        // Left-justified result (no leading 0s)
    ADCON0bits.ADCS = 1;        // Use internal Fast RC (FRC) oscillator as clock source for conversion
    ADCON0bits.ADON = 1;        // Enable ADC
}

/**********************************************************
 * ADC_getval
 * Function used to obtain ADC readings for the LDR voltage
 **********************************************************/
unsigned char ADC_getval(void) {
    unsigned char tmpval;        // Declare local variable to store voltage readings in bits
    
    ADCON0bits.GO = 1;          // Start ADC conversion
    while (ADCON0bits.GO);      // Wait until conversion done (bit is cleared automatically when done)
    tmpval = ADRESH;            // Get the 8 most significant bits of the ADC result (an 8-bit result is sufficient for our needs here)
                                // If we wanted the full 10bit result we would also need to read the 2 LSBs from ADRESL and combine the result
    
    return tmpval;              // Return this value when the function is called
}