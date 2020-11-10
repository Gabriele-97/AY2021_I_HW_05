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


#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F
#define LIS3DH_STATUS_REG 0x27
#define LIS3DH_CTRL_REG4 0x23
#define LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4 0x08 //ACTIVE HR
#define LIS3DH_XLOUT 0x28     
#define STATUS_REG 0x27  
#define HEADER 0xA0
#define TAIL 0xC0

int main(void)
{
    /* VARIABLE DEFINITION AND OUTARRAY BUFFER PREPARATION */
    
    uint8_t ctrl_reg1_ODR;
    uint8_t ctrl_reg4;
    int16 outaccx_tbt;
    int16 outaccy_tbt;
    int16 outaccz_tbt;
    uint8_t sts_reg;
    uint8_t zda;
    uint8_t yda;
    uint8_t xda;
    uint8_t zyxda;
    uint8_t OutArray[8];
    OutArray[0] = HEADER;
    OutArray[7] = TAIL;
    ErrorCode error;
    
    /* START THE PERIPHERALS AND WAIT SOME MS FOR THEIR ACTIVATION*/
    CyGlobalIntEnable; 
    I2C_Peripheral_Start();
    EEPROM_Start(); 
    UART_Start(); 
    CyDelay(5); 
     
    /* READ THE VALUE OF THE EEPROM 0X00 CELL TO DETERMINE INITIAL ODR*/ 
    EEPROM_UpdateTemperature(); //according to datasheet before reading EEPROMM
    sampling_freq = EEPROM_ReadByte(STARTUP); 
    
    
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
      
        ctrl_reg4 = LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG4;
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4);
        check_for_error(error, ctrl_reg4);
    
        
        ctrl_reg1_ODR = sampling_freq << 4;
        ctrl_reg1_ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1_ODR);
        check_for_error(error, ctrl_reg1_ODR);
    
    /* =================================================================
                                    INFINITE LOOP
        
        In the inifinite loop I control the status of the button to update 
        the sampling frequency of the accelerometer according to the specific
        value chosen by the user. Then the status register is read to check for
        the readiness of the values in the three directions that are acquired 
        and converted thanks to the proper "read and convert" function.
        Only in case all three values have been correctly acquired, they are 
        transmitted by UART protocol .
        =====================================================================*/
        
    for(;;)
    {   
        //FAI UN INTERRPUT PER IL PULSANTE ??
        updatefreq(); //this function updates the sampling frequency when the user presses a button
           
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, STATUS_REG, &sts_reg);
        zda = sts_reg & 0x04;
        yda = sts_reg & 0x02;
        xda = sts_reg & 0x01;
        zyxda = sts_reg & 0x08;
        
       
        outaccx_tbt = read_and_convert(xda, 0x28, 0x29);
        outaccy_tbt = read_and_convert(yda, 0x2A, 0x2B);
        outaccz_tbt = read_and_convert(zda, 0x2C, 0x2D);
               
        
        if(zyxda){
        OutArray[1] =  outaccx_tbt >> 8;
        OutArray[2] =  outaccx_tbt & 0xFF;
        OutArray[3] =  outaccy_tbt >> 8;
        OutArray[4] =  outaccy_tbt & 0xFF;
        OutArray[5] =  outaccz_tbt >>8;
        OutArray[6] =  outaccz_tbt &0xFF;
        
        UART_PutArray(OutArray,8);
        }            
              
    }
         
    
}


/* [] END OF FILE */
