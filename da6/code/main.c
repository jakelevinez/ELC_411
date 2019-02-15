/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    int phase;
    int dir = 1;
    uint16_t count;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {   
        for(count = 0; count < 2048/8; count++)
        {
            
            for(phase = 0; phase < 8; phase++)
            {
////                for(count = 0; count < 2048; count ++)
////                {
                    if (dir == 1) // if dir positive, Rotation = CW
                    {
                    switch (phase)
                        {
                            case 0: 
                                DigOut_Write(1); // write 0001
                                break; 
                            case 1: 
                                DigOut_Write(3); // write 0011
                                break;
                            case 2: 
                                DigOut_Write(2); // write 0010
                                break;
                            case 3:
                                DigOut_Write(6); // write 0110
                                break;
                            case 4: 
                                DigOut_Write(4); // write 0100
                                break;
                            case 5:
                                DigOut_Write(12); // write 1100
                                break;
                            case 6:
                                DigOut_Write(8); // write 1000
                                break;
                            case 7:
                                DigOut_Write(9); // write 1001
                                break;
                        }
                    }
                    
                    if (dir == 0)
                    {
                    switch (phase) // if dir negative, rotation = CCW, negate previous values
                        {
                            case 7: 
                                DigOut_Write(1); // write 0001
                                break; 
                            case 6: 
                                DigOut_Write(3); // write 0011
                                break;
                            case 5: 
                                DigOut_Write(2); // write 0010
                                break;
                            case 4:
                                DigOut_Write(6); // write 0110
                                break;
                            case 3: 
                                DigOut_Write(4); // write 0100
                                break;
                            case 2:
                                DigOut_Write(12); // write 1100
                                break;
                            case 1:
                                DigOut_Write(8); // write 1000
                                break;
                            case 0:
                                DigOut_Write(9); // write 1001
                                break;
                        }


                        
                    }
                CyDelay(10);
            }
                     
                       
        }
        CyDelay(500);
        dir = 1 - dir;
    }  
}    
