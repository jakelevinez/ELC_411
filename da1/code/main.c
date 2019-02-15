//#include "project.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

//reading value of nvic cpuid base addrs
#define NVIC_CPUID_BASE_ADDR 0xE000ED00

int main(void)
{
    // 4.1 ACCESS REGISTER VIA ITS ADDRESS
    //declaring variables
    uint32_t implementer;
    uint32_t variant;
    uint32_t partno;
    uint32_t revision;
    uint32_t reg_val;
    
    /*declaring pointer to volatile unsigned int and setting it to point to nvic cpuid base addrs*/
    uint32_t volatile * my_reg_ptr = (unsigned int *) NVIC_CPUID_BASE_ADDR;
    
    //getting value in register
    reg_val = *my_reg_ptr;
    
    //bitmasking variables
    implementer     = (reg_val >> 24) & 0xFF;
    variant         = (reg_val >> 20) & 0xF;
    partno          = (reg_val >>  4) & 0xFFF;
    revision        = (reg_val)       & 0xF;

    // 4.2 MANIPULATE A CHARACTER STRING
    //declaring string
    char myname[]= "jacob alan levine";
    
    //sorting
    int string_length = strlen(myname);
    char temp;
    int a;
    int i;
    
    for(a = 0;a<(string_length-1);a++)
    {
        for(i = 0;i<(string_length-1);i++)
        {
            if (myname[i]>myname[i+1])
            {
                temp = myname[i];
                myname[i] = myname[i+1];
                myname[i+1] = temp;
            }
        }   
    }

    // Loop forever (do nothing, for this assignment)
    for(;;)
    {
    }
}