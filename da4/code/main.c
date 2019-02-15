#include <project.h>
#include <stdio.h>


// Unsigned Fixed Point Macros, UQm.n
#define FIX_n           (16)                // fixed point 'n' value
#define FIX_m           (16)                // fixed point 'm' value
#define FIX_N           (FIX_n + FIX_m)     // total bits in UQm.n
#define FIX_FACTOR      (1   <<  FIX_n   )  // fixed point fraction factor (2^n)
#define FIX__0_5        (1   << (FIX_n-1))  // 0.5 expressed in UQ16.16
#define FIX__1_0        (1   <<  FIX_n   )  // 1.0 expressed in UQ16.16

//********************************************************************************************
//  CY_ISR ( ADDRESS ) is executed on each change seen on the input pin. Write the value of 
//  SW2_State (output) based on the value of SW2 (input)
//********************************************************************************************
CY_ISR ( SW2_HANDLER ) 
{
    SW2_State_Write(SW2_Read());      
    SW2_INT_ClearPending();
}    

//********************************************************************************************
//  fix2int takes a fix point data type and rounds it to the nearest integer, returns integer
//********************************************************************************************

uint32_t fix2int( uint32_t fix )
{
    // *****************************************************************************************
    uint32_t container; 
    container = (fix + FIX__0_5) >>  FIX_n; // 
    return container;
    // *****************************************************************************************
}

//********************************************************************************************
//  fix2double casts a fix point data type to a double, returns double
//********************************************************************************************

double fix2double( uint32_t fix )
{
    // *****************************************************************************************
    double doubleContainer;
    doubleContainer = (double)fix/FIX_FACTOR;
    return doubleContainer;
    // *****************************************************************************************
}

//********************************************************************************************
//  double2fix casts a double data type to a fixed point data type. 
//********************************************************************************************
uint32_t double2fix( double x )
{
    // *****************************************************************************************
    uint32_t container;
    container = (uint32_t)((x * FIX_FACTOR) + 0.5);
    return container;

    // *****************************************************************************************
}

void fix2decimalstr( uint32_t x, char *str, int dotn )
{
    int         i;
    uint64_t    lx;
    int         len;
    
    // Use 64 bit int to avoid overflow
    lx = x;
    
    // Multiply by 10^dotn, to shift all fractional decimal into integer part
    for (i = 0; i < dotn; ++i)
    {
        lx *= 10;
    }
    // Get the integer part via rounding by adding half, and right shifting n
    lx += FIX__0_5;
    lx >>= FIX_n;
    
    x = (uint32_t) lx;
    
    // Print the number, but without decimal point
    sprintf( str, "%d", (int) x );
    len = strlen(str);
    
    // Insert the decimal point in the correct location
    // First move all of the last 'dotn' characters to the right to make space
    str[len+1] = '\0';
    for (i = 0; i < dotn; ++i)
    {
        str[len-i] = str[len-i-1];
    }
    str[len-dotn] = '.';
}

int main()
{
    int     k;              // Current position of bouncing box (relative to LCD)
    int     direction;      // +1 --> move right, -1 --> move left
    char    num_str[17];    // Array to render the value of rate as a string
    char    msg_str[17];    // Entire message, to write to LCD
    
    int     sw2;            // Holds current switch state
    int     sw3;            // Holds current switch state
    int     sw2_prev;       // Holds previous state, for button down detection
    int     sw3_prev;       // Holds previous state, for button down detection
    
    CyGlobalIntEnable; 		// Enables global interrupts 
    
    // Enables SW2_INT (comment the line below for Part 1)
	SW2_INT_StartEx(SW2_HANDLER);
    
    uint32_t    delay =  20 * FIX__1_0;         // UQ16.16
    uint32_t    llim  =  20 * FIX__1_0;         // Upper limit of delay expressed in UQ16.16
    uint32_t    ulim  = 200 * FIX__1_0;         // Upper limit of delay expressed in UQ16.16

    uint32_t    incr = double2fix( 10.0/3.0 );  // Represent 3.33... in fixed point
    
    LCD_Display_Start();                        // Start the LCD component
    
    k = 0;          // Initialize position
    direction = 1;  // and direction
    
    sw2 = sw3 = sw2_prev = sw3_prev = 1;                    // Initialize switch states to open
    
    // Loop forever
    for(;;)
    {
        // Convert current delay to a string, with 3 decimal places precision
        fix2decimalstr(delay, num_str, 3);
        
        // Generate composite message string
        sprintf( msg_str, "Delay=%7s ms", num_str );

        // Render current state onto the display
        // Top line is bouncing square
        // Bottom line is current delay
        LCD_Display_ClearDisplay();                 // Must clear entire display before new rendering
        LCD_Display_DrawHorizontalBG(0, k, 1, 5);   // Draw the box on top line
        LCD_Display_Position(1, 0);                 // Position on bottom line
        LCD_Display_PrintString(msg_str);           // Print the msg on bottom line

        // *****************************************************************************************
        // comment this line out for Part 2. Input was renamed to SW2 and output 
		// renamed to SW2_State
		DIG_OUT_PIN_Write(DIG_IN_PIN_Read());
		// *****************************************************************************************
    
        CyDelay(fix2int(delay));                    // Delay based on integer part of delay
        k += direction;                             // Compute new position
        if (k == 15)
            direction = -1;
        else if (k == 0)
            direction = 1;
			
        // Lines below get commented out during Part 2 of the assignment. In part 1, these lines
		// are the 'polling' the input pins. Part 2 uses an ISR instead of polling
		sw2 = SW2_Read();                           // Get current switch state
        sw3 = SW3_Read();                           // Get current switch state        
		
		
        if (sw2 == 0 && sw2_prev == 1)              // If Switch 2 button down event, decrease delay
            delay -= incr;
        if (sw3 == 0 && sw3_prev == 1)              // If Switch 3 button down event, increase delay
            delay += incr;
        sw2_prev = sw2;                         // Update previous sw2 state
        sw3_prev = sw3;                         // Update previous sw3 state
		// end commenting for part 2 
		
        // Saturate delay to upper and lower limits
        if (delay > ulim) delay = ulim;
        if (delay < llim) delay = llim;
    }
}


