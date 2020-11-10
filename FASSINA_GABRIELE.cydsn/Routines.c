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
#include "Routines.h"

void check_for_error(ErrorCode error, uint8_t register_value){
    
    char message[50];
    
    if (error == NO_ERROR)
    {
        sprintf(message, " REG: 0x%02X \r\n", register_value);
        UART_PutString(message); 
    }
    else
    {
        UART_PutString("Error occurred during I2C comm\r\n");   
    }

}


void updatefreq(){
        uint8_t ODR;
        if (PushButton_Read() == PUSH_BUTTON_PRESSED){
        while (PushButton_Read() == PUSH_BUTTON_PRESSED);
        sampling_freq ++;
                if (sampling_freq > 6) sampling_freq = 1;
                if (sampling_freq <1) sampling_freq =1;
        
                EEPROM_WriteByte(sampling_freq,STARTUP); //metto la nuova frequenza nella EEprom 
                ODR = sampling_freq << 4 ;
                ODR |= LIS3DH_HIGH_RESOLUTION_MODE_CTRL_REG1;
                
        ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ODR);
    
        check_for_error(error,ODR);
}
}

int16 read_and_convert(uint8_t da, uint8_t reg_low, uint8_t reg_high){
    uint8_t acc[2];
    int16 outacc;
    float outacc_conv;
    int16 outacc_tbt;
    if(da){        
        I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, reg_low, &acc[0]);
        I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, reg_high, &acc[1]);
        outacc = (int16)((acc[0] | (acc[1]<<8)))>>4;
        if(outacc > 2048) outacc = 2048; 
        if(outacc < -2048) outacc = -2048; 
        outacc_conv = 0.001 * outacc  ;
        outacc_conv = outacc_conv * 9.81;
        outacc_tbt = (int16) outacc_conv * 1000;
        }
   return outacc_tbt;

}

/* [] END OF FILE */
/* [] END OF FILE */
