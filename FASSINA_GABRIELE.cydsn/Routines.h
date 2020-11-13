/* ========================================
 *
 * Copyright LTEBS 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Gabriele Fassina.
 *
 * ========================================
*/


#ifndef __ROUTINES_H_
    #define __ROUTINES_H_
    
    #include "project.h"
    #include "I2C_Interface.h"
    
    #define PUSH_BUTTON_PRESSED 0 //pull-up configuration, i.e. button pressed for low value
    #define BYTE_TO_TRANSMIT 8
    #define STARTUP 0x00 //startup frequency regiser in eeprom
    #define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1 0x07 //sensor enable and LPEN bit down
    #define LIS3DH_DEVICE_ADDRESS 0x18
    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 0x08 //ACTIVE HR
    #define LIS3DH_STATUS_REG 0x27
    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_XLOUT 0x28   
    #define LIS3DH_XHIGH 0x29   
    #define LIS3DH_YLOUT 0x2A   
    #define LIS3DH_YHIGH 0x2B   
    #define LIS3DH_ZLOUT 0x2C   
    #define LIS3DH_ZHIGH 0x2D   
    #define STATUS_REG 0x27 
    #define ZYXDA_MASK 0x08
    #define MAX_FREQUENCY 0x06
    #define MIN_FREQUENCY 0x01
    #define MAX_DIGIT 2048
    #define MIN_DIGIT -2048

    #define HEADER 0xA0
    #define TAIL 0xC0
    
    uint8_t ctrl_reg1_ODR;
    uint8_t ctrl_reg4;
    int16 outaccx_tbt;
    int16 outaccy_tbt;
    int16 outaccz_tbt;
    uint8_t sts_reg;
    uint8_t zyxda;
    uint8_t OutArray[BYTE_TO_TRANSMIT];
    ErrorCode error;

    
    #define G 9.81 //convertion factor to m/s^2
    #define K 0.001 //conversion factor from digits to g as indicated in the datasheet
    #define TRICK 1000 //value t keep 3 digits
    
    /* ==================================================================================
                                    CHECK FOR ERROR FUNCTION
    
    This function is aimed at controlling if the values of the registers have
    been correctly set. It receives the error variable defined in a custom errocode
    type that can be either Error or Noerror and the value of the register that is 
    supposed to be written/read. it produces in output a string that is sent by UART
    to check the correctness of the transmission.
    ==========================================================================*/
    void check_for_error(ErrorCode error, uint8_t register_value);
    
    /* ================================================================================
                                UPDATEFREQ FUNCTION
    
    This function is aimed at updating the sampling frequency of the accelerometer as 
    the user presses a button. 6 different configurations can be considered and set
    by controlling three bits in the control register 1. This function both updates 
    the register value and also the EEPROM register chosen to store the sampling frequency.
    In this way, the following restart of the device will start back from the last
    ODR that had been chosen. Finally, the function gives in output a string(by UART) 
    indicating the frequency that has been chosen or an error message if the system 
    hasn't worked as intended.
    ====================================================================================*/
    void updatefreq();
    
    /* ===============================================================================================
                                        READ AND CONVERT FUNCTION
    
    This function converts the data read by the accelerometer in a given direction and stored
    in two 8-bits registers into the output value expressed in m/s^2. To do so, firstly the two
    data are merged in a 12 bit variable. then a control on the upper and lower limits (+-2^12/2) 
    is performed. A conversion from digits to g values is done according to K factor that is set
    now to one mg according the indication in the datasheet for an FS = +-2. The conversion to m/s^2
    is obtained just by multiplying per g = 9.81. In conclusion a trick is applied to save 3 
    decimals in the transmission (i.e. x1000 multiplication). To retrieve them, the Bridge Control Panel 
    is set with a scale of 0.001. 
    I have decided to keep the acquisitions and conversions of three directions separate (instead of doing
    a multiread) because it could be usefult in case the user wanted to have just a planar information. 
    FINAL RMK: since I'm multiplying and dividing times 1000, I could have avoided this passages but for seek 
    of generalizability, in case in the future the developer would like to change these parameters (number of 
    digits and FS), it would be easier to do so with the function is implemented this way.
    ==========================================================================================*/
    int16 read_and_convert(uint8_t reg_low, uint8_t reg_high);
    
    /* =============================================================================================
                                        TRANSMIT FUNCTION
    
    This function receives in input the values read and convert and produces in output the transmission
    packet. In particular the 16 bits are sent in two parts of 8 bits each. 
    ===============================================================================================*/
    void transmit(int16 accx, int16 accy, int16 accz); 
    
     /* ==============================================================================                     
                            HIGH RESOLUTION MODE SETTING
    
    According to design requirement, I have to set the resolution mode as high,
    i.e. to have 12 bits values from the accelerometer. To do so, I have set the 
    bit 3 of CONTROL REGISTER 1 (named LPEN) to 0 and bit 3 of CONTROL REGISTER 4
    (named HR) to 1. CONTROL REGISTER 1 also contains the enable of the accelerometer
    that is set to one in all directions. Besides, it also contains the ODR values
    that are here set at the initial value according to the configuration read in 
    the EEPROM memory
    ================================================================================*/
    void high_resolution_set();
    
    volatile uint8_t sampling_freq;
    uint8_t OutArray[BYTE_TO_TRANSMIT];
    
#endif


/* [] END OF FILE */
