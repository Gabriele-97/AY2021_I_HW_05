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


#ifndef __ROUTINES_H_
    #define __ROUTINES_H_
    
    #include "project.h"
    #include "I2C_Interface.h"
    
    #define PUSH_BUTTON_PRESSED 0
    #define STARTUP 0x00
    #define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1 0x07
    #define LIS3DH_DEVICE_ADDRESS 0x18
    #define LIS3DH_CTRL_REG1 0x20
    
    #define G 9.81
    #define K 0.001
    #define TRICK 1000;
    
    void check_for_error(ErrorCode error, uint8_t register_value);
    void updatefreq();
    int16 read_and_convert(uint8_t da, uint8_t reg_low, uint8_t reg_high);
    volatile uint8_t sampling_freq;
    
#endif


/* [] END OF FILE */
