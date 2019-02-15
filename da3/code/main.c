
#include "project.h"
#include "stdio.h"

const double maxVo = 2.048; // student added constant for calculations
const uint8_t maxCounts = 255; // student added constant for calculations

CY_ISR_PROTO(my_isr);

CY_ISR(my_isr)
{
    static int count = 0;   // must be static, so value retained between interrupts, one time initialization
    
    //****************************************************************************************        
    // Add code here to alternately drive VDAC8 to 1/4 full scale and 3/4 full scale
    //
    //****************************************************************************************        
    
    if (count == 0)
        VDAC8_SetValue(maxCounts / 4);
        
    if (count == 1)
        VDAC8_SetValue(maxCounts * 3 / 4);   
    // Don't change code below
    isr_1_ClearPending();
    count = 1 - count;      // toggle between 0 and 1
}

int
main( void )

{
    int     sar_result;     // holds result from ADC_SAR
    int     volts_int;      // integer part of volts, for string formatting to overcome bug in PSoC Creator
    int     volts_frac;     // fractional part of volts, for string formatting to overcome bug in PSoC Creator
    char    disp_str[17];   // char array large enough to hold one line for display
    
    double  volts;          // student computes this, as a function of sar_result
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Initialization, start your engines ...
    ADC_SAR_Start();
    ADC_SAR_StartConvert();     // Needed because ADC_SAR is free-running
    VDAC8_Start();
    isr_1_StartEx(my_isr);
    LCD_Char_Start();
    
    // Loop forever
    while (SW3_Read() == 1)
    {
        sar_result = ADC_SAR_GetResult16();                         // get new ADC value
        
        //****************************************************************************************        
        // Uncomment and fill in code below, to convert sar_result to the corresponding (floating point) voltage
        //
        volts = ADC_SAR_CountsTo_Volts(sar_result);
        //

<<LP -- yes, but what is this function??
        
        //****************************************************************************************        
        
        // Don't change anything below!
        volts_int = (int) volts;                                    // get integer part
        volts_frac = (int) ((volts - volts_int) * 1000 + 0.5);      // get fractional part as a 3-digit integer
        
        // Display the string on top line, left justified
        LCD_Char_ClearDisplay();
        LCD_Char_Position(0, 0);
        sprintf( disp_str, "code=%4d", sar_result);
        LCD_Char_PrintString(disp_str);

        // Display the string on bottom line, left justified
        LCD_Char_Position(1, 0);
        sprintf( disp_str, "volts=%d.%03d", volts_int, volts_frac );
        LCD_Char_PrintString(disp_str);
        
        CyDelay(500);                                               // a little time for display to stabilize
    }
}

<<LP 9/10
