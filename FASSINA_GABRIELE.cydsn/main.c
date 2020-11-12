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
#include "project.h"
#include "I2C_Interface.h"
#include "Routines.h"

/* =========================================================================
                                    READ ME
In this assignment, I was asked to develop the code for an accelerometer working in
high resolution mode (i.e. 12 bits data) whose sampling frequency is changed by button pressing.
To do so, I have written some custom function whose functionalities are described in the 
Routines.h file. In the main function, after the initialization of the peripherals, 
I acquire the starting sampling frequency saved in the EEPROM memory. by a proper function, the 
high resolution mode is set. In the infinite loop, a control is performed on button pushed and, when available
data are acquired and transmitted by UART protocol. The control on button pushed is such a rare event that 
it doesn't affect the performance of the device but for the occurrences of frequency change. However, 
I have preferred to write this funnction without the need of an interrupt to keep using the 
Putstring function (which could not be introduced in interrupts because blocking) to check the correctness
of the frequency set. 
===============================================================================*/

int main(void)
{ 
    /* START THE PERIPHERALS AND WAIT SOME MS FOR THEIR ACTIVATION*/
    CyGlobalIntEnable; 
    UART_Start(); 
    I2C_Peripheral_Start();
    EEPROM_Start(); 
    OutArray[0] = HEADER;
    OutArray[BYTE_TO_TRANSMIT -1] = TAIL;
    CyDelay(5); 
     
    /* READ THE VALUE OF THE EEPROM 0X00 CELL TO DETERMINE INITIAL ODR*/ 
    EEPROM_UpdateTemperature(); //according to datasheet before reading EEPROMM
    sampling_freq = EEPROM_ReadByte(STARTUP); 
    if (sampling_freq >0x06 || sampling_freq <0x01) sampling_freq = 0X01;
    
    high_resolution_set();
    
        
    for(;;)
    {   
        updatefreq(); //this function updates the sampling frequency when the user presses a button
           
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, STATUS_REG, &sts_reg);
        zyxda = sts_reg & ZDA_MASK ;
                
        if(zyxda){
        outaccx_tbt = read_and_convert(LIS3DH_XLOUT, LIS3DH_XHIGH);
        outaccy_tbt = read_and_convert(LIS3DH_YLOUT, LIS3DH_YHIGH);
        outaccz_tbt = read_and_convert(LIS3DH_ZLOUT, LIS3DH_ZHIGH);      
        transmit(outaccx_tbt,outaccy_tbt,outaccz_tbt);
        }            
              
    } 
    
}

